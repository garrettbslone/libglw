//
// Created by Garrett on 3/27/22.
//

#include <array>
#include "vk_renderer.hpp"
#include "vk_framebuffer.hpp"
#include "../../util/exception.hpp"
#include "../descriptor.hpp"

namespace glw {

vk_renderer::vk_renderer(window *w)
        : renderer(w),
          swap_chain_(nullptr),
          pipeline_manager_(nullptr),
          global_pool_(nullptr),
          global_set_layout_(nullptr)
{
    if (!(this->device_ = vk_device::get()))
        throw vulkan_device_ex("A vulkan device is required to create a vulkan renderer!");
}

vk_renderer::~vk_renderer()
{
    delete this->pipeline_manager_;
}

void vk_renderer::init()
{
    this->recreate_swap_chain();
    this->create_command_buffers();
    this->build_descriptor_sets();

    this->pipeline_manager_ = new vk_pipeline_manager(this->global_set_layout_, this->swap_chain_);
}

float vk_renderer::get_aspect_ratio() const
{
    return this->swap_chain_->extent_aspect_ratio();
}

command_buffer *vk_renderer::begin_frame()
{
    assert(!this->is_frame_started_ && "Can't call begin_frame while already in progress");

    auto res = this->swap_chain_->acquire_next_image(&this->current_image_index_);

    if (res == VK_ERROR_OUT_OF_DATE_KHR) {
        this->recreate_swap_chain();
        return nullptr;
    } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
        throw vulkan_image_ex("failed to acquire swap chain image!");
    }

    this->is_frame_started_ = true;

    auto cmd_buffer = get_current_command_buffer();
    VkCommandBufferBeginInfo begin_info{};

    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(static_cast<VkCommandBuffer>(cmd_buffer->raw()), &begin_info) != VK_SUCCESS)
        throw vulkan_image_ex("failed to begin recording command buffer!");

    return cmd_buffer;
}

void vk_renderer::end_frame()
{
    assert(this->is_frame_started_ && "Can't call end_frame while frame is not in progress");

    auto cmd_buffer = get_current_command_buffer();
    auto _cmds = static_cast<VkCommandBuffer>(cmd_buffer->raw());
    if (vkEndCommandBuffer(_cmds) != VK_SUCCESS)
        throw vulkan_image_ex("failed to record command buffer!");

    auto res = this->swap_chain_->submit_command_buffers(&_cmds, &this->current_image_index_);

    if (res == VK_ERROR_OUT_OF_DATE_KHR || this->window_->is_framebuffer_resized()) {
        this->window_->set_framebuffer_resized(false);
        recreate_swap_chain();
    } else if (res != VK_SUCCESS)
        throw vulkan_image_ex("failed to acquire swap chain image!");

    this->is_frame_started_ = false;
    this->current_frame_index_ = (this->current_frame_index_ + 1) % swap_chain::MAX_FRAMES_IN_FLIGHT;
}

void vk_renderer::begin_swap_chain_render_pass(command_buffer *cmd_buffer)
{
    assert(this->is_frame_started_ &&
        "Can't call begin_swap_chain_render_pass if frame not in progress");
    assert(cmd_buffer == get_current_command_buffer() &&
        "Can't begin render pass on command buffer from a different frame!");

    auto _cmds = static_cast<VkCommandBuffer>(cmd_buffer->raw());
    VkRenderPassBeginInfo render_pass_info{};

    VkRect2D scissor{{0, 0}, this->swap_chain_->get_swap_chain_extent()};
    VkViewport viewport{};

    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = static_cast<float>(this->swap_chain_->get_swap_chain_extent().width);
    viewport.height = static_cast<float>(this->swap_chain_->get_swap_chain_extent().height);
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    vkCmdSetViewport(_cmds, 0, 1, &viewport);
    vkCmdSetScissor(_cmds, 0, 1, &scissor),

    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = this->swap_chain_->get_render_pass();
    render_pass_info.framebuffer =
            this->swap_chain_->get_frame_buffer(this->current_frame_index_)->framebuffer_;
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = this->swap_chain_->get_swap_chain_extent();

    std::array<VkClearValue, 2> clear_vals{};

    clear_vals[0].color = {0.01f, 0.01f, 0.01f, 1.f};
    clear_vals[1].depthStencil = {1.f, 0};

    render_pass_info.clearValueCount = static_cast<uint32_t>(clear_vals.size());
    render_pass_info.pClearValues = clear_vals.data();

    vkCmdBeginRenderPass(static_cast<VkCommandBuffer>(cmd_buffer->raw()),
            &render_pass_info,
            VK_SUBPASS_CONTENTS_INLINE);
}

void vk_renderer::end_swap_chain_render_pass(command_buffer *cmd_buffer)
{
    assert(this->is_frame_started_ &&
        "Can't call end_swap_chain_render_pass if frame is not in progress");
    assert(cmd_buffer == get_current_command_buffer() &&
        "Can't end_swap_chain_render_pass on command buffer from a different frame!");
    vkCmdEndRenderPass(static_cast<VkCommandBuffer>(cmd_buffer->raw()));
}

void vk_renderer::create_command_buffers()
{
    this->command_buffers_.resize(swap_chain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo alloc_info{};

    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = this->device_->get_command_pool();
    alloc_info.commandBufferCount = static_cast<uint32_t>(this->command_buffers_.size());

    this->raw_buffers_.reserve(this->command_buffers_.size());

    for (auto &buff: this->command_buffers_) {
        this->raw_buffers_.push_back(static_cast<VkCommandBuffer>(buff->raw()));
    }

    if (vkAllocateCommandBuffers(
            this->device_->device_,
            &alloc_info,
            this->raw_buffers_.data()) != VK_SUCCESS)
        throw vulkan_buffer_ex("failed to allocate command buffers");
}

void vk_renderer::free_command_buffers()
{
    vkFreeCommandBuffers(
            this->device_->device_,
            this->device_->get_command_pool(),
            static_cast<uint32_t>(this->raw_buffers_.size()),
            this->raw_buffers_.data());

    this->command_buffers_.clear();
    this->raw_buffers_.clear();
}

void vk_renderer::recreate_swap_chain()
{
    VkExtent2D extent = {
            static_cast<uint32_t>(this->window_->get_width()),
            static_cast<uint32_t>(this->window_->get_height())
    };

    // we don't want a swap chain image that won't be visible, for example
    // if the window is minimized we don't want to try drawing to it
    while (extent.width == 0 || extent.height == 0) {
        extent = {
                static_cast<uint32_t>(this->window_->get_width()),
                static_cast<uint32_t>(this->window_->get_height())
        };

        glfwWaitEvents();
    }

    vkDeviceWaitIdle(this->device_->device_);

    if (!this->swap_chain_)
        this->swap_chain_ = new vk_swap_chain(extent);
    else {
        vk_swap_chain *old_swap_chain = this->swap_chain_;
        this->swap_chain_ = new vk_swap_chain(extent, old_swap_chain);

        if (!old_swap_chain->compare_swap_formats(*this->swap_chain_))
            throw vulkan_image_ex("Swap chain image(or depth) format has changed!");
    }
}

command_buffer *vk_renderer::get_current_command_buffer() const
{
    assert(this->is_frame_started_ &&
        "Cannot get command buffer when frame not in progress!");
    return this->command_buffers_[this->current_frame_index_];
}

void vk_renderer::build_descriptor_sets()
{
    this->global_pool_ = vk_descriptor_pool::builder()
            .set_max_sets(swap_chain::MAX_FRAMES_IN_FLIGHT)
            .add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain::MAX_FRAMES_IN_FLIGHT)
            .build();

    this->global_set_layout_ = vk_descriptor_set_layout::builder()
            .add_binding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

    this->uniform_buffers_.reserve(swap_chain::MAX_FRAMES_IN_FLIGHT);

    for (auto i = 0; i < swap_chain::MAX_FRAMES_IN_FLIGHT; i++) {
        this->uniform_buffers_.push_back(
                new vk_buffer(
                    sizeof(global_ubo),
                    1,
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
        this->uniform_buffers_[i]->map();
    }

    this->global_descriptor_sets_.reserve(swap_chain::MAX_FRAMES_IN_FLIGHT);

    for (auto i = 0; i < swap_chain::MAX_FRAMES_IN_FLIGHT; i++) {
        auto buffer_info = this->uniform_buffers_[i]->descriptor_info();
        vk_descriptor_writer(*this->global_set_layout_, *this->global_pool_)
                .write_buffer(0, &buffer_info)
                .build(this->global_descriptor_sets_[i]);
    }
}

void *vk_renderer::get_descriptor_set()
{
    return this->global_descriptor_sets_[this->current_frame_index_];
}

}
