//
// Created by Garrett on 3/29/22.
//

#ifndef GLW_VK_SWAP_CHAIN_HPP
#define GLW_VK_SWAP_CHAIN_HPP

#include "vk_device.hpp"
#include "../swap_chain.hpp"

#include <memory>
#include <string>
#include <vector>

namespace glw {

class vk_framebuffer;

class vk_swap_chain : public swap_chain {
public:
    vk_swap_chain(VkExtent2D window_extent);
    vk_swap_chain(VkExtent2D window_extent, vk_swap_chain *previous);
    ~vk_swap_chain() override;

    vk_swap_chain(const vk_swap_chain &) = delete;
    vk_swap_chain &operator=(const vk_swap_chain &) = delete;

private:
    vk_framebuffer *get_frame_buffer(int index) { return swap_chain_framebuffers_[index]; }
    VkRenderPass get_render_pass() { return render_pass_; }
    VkImageView get_image_view(int index) { return swap_chain_image_views_[index]; }
    size_t image_count() { return swap_chain_images_.size(); }
    VkFormat get_swap_chain_image_format() { return swap_chain_image_format_; }
    VkExtent2D get_swap_chain_extent() { return swap_chain_extent_; }
    uint32_t width() { return swap_chain_extent_.width; }
    uint32_t height() { return swap_chain_extent_.height; }

    float extent_aspect_ratio()
    {
        return static_cast<float>(swap_chain_extent_.width) / static_cast<float>(swap_chain_extent_.height);
    }
    VkFormat find_depth_format();

    VkResult acquire_next_image(uint32_t *image_index);
    VkResult submit_command_buffers(const VkCommandBuffer *buffers, uint32_t *image_index);

    bool compare_swap_formats(const vk_swap_chain &chain) const
    {
        return chain.swap_chain_image_format_ == this->swap_chain_image_format_ &&
                chain.swap_chain_depth_format == this->swap_chain_depth_format;
    }

    static uint8_t swap_chain_image;

    void init();

    void create_swap_chain();
    void create_render_pass();
    void create_sync_objects();

    // Helper functions
    VkSurfaceFormatKHR choose_swap_surface_format(
            const std::vector<VkSurfaceFormatKHR> &available_formats);
    VkPresentModeKHR choose_swap_present_mode(
            const std::vector<VkPresentModeKHR> &available_present_modes);
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities);

    VkFormat swap_chain_image_format_;
    VkFormat swap_chain_depth_format;
    VkExtent2D swap_chain_extent_;

    std::vector<vk_framebuffer *> swap_chain_framebuffers_;
    VkRenderPass render_pass_;

    std::vector<VkImage> swap_chain_images_;
    std::vector<VkImageView> swap_chain_image_views_;

    vk_device *device_;
    VkExtent2D window_extent_;

    VkSwapchainKHR swap_chain_;
    vk_swap_chain *old_swap_chain_;

    std::vector<VkSemaphore> image_available_semaphores_;
    std::vector<VkSemaphore> render_finished_semaphores_;
    std::vector<VkFence> in_flight_fences_;
    std::vector<VkFence> images_in_flight_;

    size_t current_frame_ = 0;

    friend class vk_framebuffer;
    friend class vk_buffer;
    friend class vk_descriptor_set_layout;
    friend class vk_descriptor_pool;
    friend class vk_descriptor_writer;
    friend class vk_framebuffer;
    friend class vk_pipeline;
    friend class vk_pipeline_manager;
    friend class vk_renderer;
    friend class vk_shader;
};

}

#endif //GLW_VK_SWAP_CHAIN_HPP
