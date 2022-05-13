//
// Created by Garrett on 3/29/22.
//

#include "vk_swap_chain.hpp"
#include "../../util/exception.hpp"

#include <array>
#include <cstdlib>
#include <iostream>
#include <limits>

namespace glw {

uint8_t vk_swap_chain::swap_chain_image = 0;

vk_swap_chain::vk_swap_chain(vk_device *dev, VkExtent2D window_extent)
        : device_{dev}, window_extent_{window_extent}
{
    init();
}

vk_swap_chain::vk_swap_chain(vk_device *dev, VkExtent2D window_extent, vk_swap_chain *previous)
        : device_{dev}, window_extent_{window_extent}, old_swap_chain_{previous}
{
    init();
}

void vk_swap_chain::init()
{
    create_swap_chain();
    create_render_pass();
    create_sync_objects();
}

vk_swap_chain::~vk_swap_chain() {
    vkDeviceWaitIdle(this->device_->device_);

    if (this->swap_chain_ != nullptr) {
        vkDestroySwapchainKHR(this->device_->device_, this->swap_chain_, nullptr);
        this->swap_chain_ = nullptr;
    }

    for (auto fb : this->swap_chain_framebuffers_) {
        delete fb;
    }

    vkDestroyRenderPass(this->device_->device_, this->render_pass_, nullptr);
    // cleanup synchronization objects
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(this->device_->device_, this->render_finished_semaphores_[i], nullptr);
        vkDestroySemaphore(this->device_->device_, this->image_available_semaphores_[i], nullptr);
        vkDestroyFence(this->device_->device_, this->in_flight_fences_[i], nullptr);
    }
}

VkResult vk_swap_chain::acquire_next_image(uint32_t *image_index) {
    vkWaitForFences(
            this->device_->device_,
            1,
            &this->in_flight_fences_[current_frame_],
            VK_TRUE,
            std::numeric_limits<uint64_t>::max());

    VkResult result = vkAcquireNextImageKHR(
            this->device_->device_,
            this->swap_chain_,
            std::numeric_limits<uint64_t>::max(),
            this->image_available_semaphores_[current_frame_],  // must be a not signaled semaphore
            VK_NULL_HANDLE,
            image_index);

    return result;
}

VkResult vk_swap_chain::submit_command_buffers(
        const VkCommandBuffer *buffers, uint32_t *image_index) {
    if (this->images_in_flight_[*image_index] != VK_NULL_HANDLE)
        vkWaitForFences(this->device_->device_, 1, &this->images_in_flight_[*image_index], VK_TRUE, UINT64_MAX);

    this->images_in_flight_[*image_index] = this->in_flight_fences_[this->current_frame_];

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {this->image_available_semaphores_[this->current_frame_]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = buffers;

    VkSemaphore signalSemaphores[] = {this->render_finished_semaphores_[this->current_frame_]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(this->device_->device_, 1, &this->in_flight_fences_[this->current_frame_]);
    if (vkQueueSubmit(this->device_->graphics_queue(), 1, &submitInfo, this->in_flight_fences_[this->current_frame_]) !=
            VK_SUCCESS) {
        throw vulkan_device_ex("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swap_chain_};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = image_index;

    auto result = vkQueuePresentKHR(this->device_->present_queue(), &presentInfo);

    current_frame_ = (current_frame_ + 1) % MAX_FRAMES_IN_FLIGHT;

    return result;
}

void vk_swap_chain::create_swap_chain() {
    swap_chain_support_details swapChainSupport = this->device_->get_swap_chain_support();

    VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swapChainSupport.formats);
    VkPresentModeKHR presentMode = choose_swap_present_mode(swapChainSupport.present_modes);
    VkExtent2D extent = choose_swap_extent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = this->device_->surface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    queue_family_indices indices = this->device_->find_physical_queue_families();
    uint32_t queueFamilyIndices[] = {indices.graphics_family, indices.present_family};

    if (indices.graphics_family != indices.present_family) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;      // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = !old_swap_chain_ ? VK_NULL_HANDLE : old_swap_chain_->swap_chain_;

    auto res = vkCreateSwapchainKHR(this->device_->device_, &createInfo, nullptr, &swap_chain_);
    if (res != VK_SUCCESS) {
        std::cout << "err: " << res << "\n";
        throw std::runtime_error("failed to create swap chain! ");
    }

    // we only specified a minimum number of images in the swap chain, so the implementation is
    // allowed to create a swap chain with more. That's why we'll first query the final number of
    // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
    // retrieve the handles.
    vkGetSwapchainImagesKHR(this->device_->device_, swap_chain_, &imageCount, nullptr);
    swap_chain_images_.resize(imageCount);
    vkGetSwapchainImagesKHR(this->device_->device_, swap_chain_, &imageCount, swap_chain_images_.data());

    swap_chain_image_format_ = surfaceFormat.format;
    swap_chain_extent_ = extent;
}

void vk_swap_chain::create_render_pass() {
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = find_depth_format();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = get_swap_chain_image_format();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstSubpass = 0;
    dependency.dstStageMask =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(this->device_->device_, &renderPassInfo, nullptr, &render_pass_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void vk_swap_chain::create_sync_objects() {
    image_available_semaphores_.resize(MAX_FRAMES_IN_FLIGHT);
    render_finished_semaphores_.resize(MAX_FRAMES_IN_FLIGHT);
    in_flight_fences_.resize(MAX_FRAMES_IN_FLIGHT);
    images_in_flight_.resize(image_count(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(this->device_->device_, &semaphoreInfo, nullptr, &image_available_semaphores_[i]) !=
                VK_SUCCESS ||
                vkCreateSemaphore(this->device_->device_, &semaphoreInfo, nullptr, &render_finished_semaphores_[i]) !=
                        VK_SUCCESS ||
                vkCreateFence(this->device_->device_, &fenceInfo, nullptr, &in_flight_fences_[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

VkSurfaceFormatKHR vk_swap_chain::choose_swap_surface_format(
        const std::vector<VkSurfaceFormatKHR> &available_formats) {
    for (const auto &availableFormat : available_formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR vk_swap_chain::choose_swap_present_mode(
        const std::vector<VkPresentModeKHR> &available_present_modes) {
    for (const auto &availablePresentMode : available_present_modes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            std::cout << "Present mode: Mailbox" << std::endl;
            return availablePresentMode;
        }
    }

    // for (const auto &availablePresentMode : available_present_modes) {
    //   if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
    //     std::cout << "Present mode: Immediate" << std::endl;
    //     return availablePresentMode;
    //   }
    // }

    std::cout << "Present mode: V-Sync" << std::endl;
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D vk_swap_chain::choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = window_extent_;
        actualExtent.width = std::max(
                capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(
                capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

VkFormat vk_swap_chain::find_depth_format()
{
    return this->device_->find_supported_format(
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

}
