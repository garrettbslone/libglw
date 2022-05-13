//
// Created by Garrett on 3/29/22.
//

#ifndef GLW_VK_FRAMEBUFFER_HPP
#define GLW_VK_FRAMEBUFFER_HPP

#include "vk_swap_chain.hpp"
#include "../framebuffer.hpp"

namespace glw {

class vk_framebuffer : public framebuffer {
public:
    vk_framebuffer(swap_chain *chain);
    ~vk_framebuffer() override;

    void init() override;
    void clear() override;
    void destroy() override;
    void reset() override;

    void bind() override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height) override;

private:
    void create_image_view();
    void create_depth_resources();
    void create_framebuffer();

    vk_swap_chain *swap_chain_;
    VkFramebuffer framebuffer_;

    VkImage depth_image_;
    VkDeviceMemory depth_image_memory_;
    VkImageView depth_image_view_;
    VkImageView swap_chain_image_view_;

    friend class vk_renderer;
};

}

#endif //GLW_VK_FRAMEBUFFER_HPP
