//
// Created by Garrett on 3/27/22.
//

#ifndef GLW_VK_RENDERER_HPP
#define GLW_VK_RENDERER_HPP

#include "../renderer.hpp"

#include "vk_buffer.hpp"
#include "vk_descriptor.hpp"
#include "vk_pipeline_manager.hpp"
#include "vk_swap_chain.hpp"

namespace glw {

class vk_renderer : public renderer {
public:
    vk_renderer(window *w, device *d);
    ~vk_renderer() override;

    void init() override;
    float get_aspect_ratio() const override;
    command_buffer *begin_frame() override;
    void end_frame() override;
    void begin_swap_chain_render_pass(command_buffer *cmd_buffer) override;
    void end_swap_chain_render_pass(command_buffer *cmd_buffer) override;

    void *get_descriptor_set() override;

protected:
    void create_command_buffers() override;
    void free_command_buffers() override;
    void recreate_swap_chain() override;

private:
    command_buffer *get_current_command_buffer() const;
    void build_descriptor_sets();

    vk_device *device_;
    vk_swap_chain *swap_chain_;
    vk_pipeline_manager *pipeline_manager_;

    vk_descriptor_pool *global_pool_;
    vk_descriptor_set_layout *global_set_layout_;
    std::vector<vk_buffer *> uniform_buffers_;
    std::vector<VkDescriptorSet> global_descriptor_sets_;

    std::vector<VkCommandBuffer> raw_buffers_;
};

}

#endif //GLW_VK_RENDERER_HPP
