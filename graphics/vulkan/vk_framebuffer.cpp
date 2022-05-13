//
// Created by Garrett on 3/29/22.
//

#include "vk_framebuffer.hpp"
#include "../../util/exception.hpp"

#include <array>

namespace glw {

vk_framebuffer::vk_framebuffer(swap_chain *chain)
{
    if (!(this->swap_chain_ = dynamic_cast<vk_swap_chain *>(chain)))
        throw vulkan_load_ex("vulkan swap chain is required for a vulkan framebuffer!");

    this->create_image_view();
    this->create_depth_resources();
    this->create_framebuffer();
}

vk_framebuffer::~vk_framebuffer()
{
    vkDestroyImageView(this->swap_chain_->device_->device_, this->swap_chain_image_view_, nullptr);
    vkDestroyImageView(this->swap_chain_->device_->device_, this->depth_image_view_, nullptr);
    vkDestroyImage(this->swap_chain_->device_->device_, this->depth_image_, nullptr);
    vkFreeMemory(this->swap_chain_->device_->device_, this->depth_image_memory_, nullptr);
    vkDestroyFramebuffer(this->swap_chain_->device_->device_, this->framebuffer_, nullptr);
}

void vk_framebuffer::init()
{
    this->create_image_view();
    this->create_depth_resources();
    this->create_framebuffer();
}

void vk_framebuffer::clear()
{

}

void vk_framebuffer::destroy()
{

}

void vk_framebuffer::reset()
{

}

void vk_framebuffer::bind()
{

}

void vk_framebuffer::unbind()
{

}

void vk_framebuffer::resize(uint32_t width, uint32_t height)
{

}

void vk_framebuffer::create_image_view()
{
    VkImageViewCreateInfo viewInfo{};

    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = this->swap_chain_->swap_chain_images_[vk_swap_chain::swap_chain_image++];
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = this->swap_chain_->swap_chain_image_format_;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(this->swap_chain_->device_->device_,
            &viewInfo,
            nullptr,
            &swap_chain_image_view_) !=
            VK_SUCCESS)
        throw vulkan_load_ex("failed to create texture image view!");
}

void vk_framebuffer::create_depth_resources()
{
    VkFormat depthFormat = this->swap_chain_->find_depth_format();
    this->swap_chain_->swap_chain_depth_format = depthFormat;
    VkExtent2D swapChainExtent = this->swap_chain_->get_swap_chain_extent();

    VkImageCreateInfo imageInfo{};

    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = swapChainExtent.width;
    imageInfo.extent.height = swapChainExtent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = depthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.flags = 0;

    this->swap_chain_->device_->create_image_with_info(
            imageInfo,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            depth_image_,
            depth_image_memory_);

    VkImageViewCreateInfo viewInfo{};

    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = depth_image_;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = depthFormat;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(this->swap_chain_->device_->device_,
            &viewInfo,
            nullptr,
            &depth_image_view_) != VK_SUCCESS)
        throw std::runtime_error("failed to create texture image view!");
}

void vk_framebuffer::create_framebuffer()
{
    std::array<VkImageView, 2> attachments = {
            this->swap_chain_image_view_,
            this->depth_image_view_
    };

    VkExtent2D swapChainExtent = this->swap_chain_->get_swap_chain_extent();
    VkFramebufferCreateInfo framebufferInfo = {};

    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = this->swap_chain_->render_pass_;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = swapChainExtent.width;
    framebufferInfo.height = swapChainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(
            this->swap_chain_->device_->device_,
            &framebufferInfo,
            nullptr,
            &this->framebuffer_) != VK_SUCCESS)
        throw std::runtime_error("failed to create framebuffer!");
}

}
