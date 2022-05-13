//
// Created by Garrett on 3/29/22.
//

#ifndef GLW_VK_PIPELINE_HPP
#define GLW_VK_PIPELINE_HPP

#include "../pipeline.hpp"

#include "vk_buffer.hpp"
#include "vk_shader.hpp"

namespace glw {

struct push_constant_data {
  glm::mat4 model_matrix_{1.f};
  glm::mat4 normal_matrix_{1.f};
};

class vk_pipeline;

class vk_pipeline_config_info : public pipeline_config_info {
public:
    vk_pipeline_config_info();

    vk_pipeline_config_info(const vk_pipeline_config_info &) = delete;
    vk_pipeline_config_info &operator=(const vk_pipeline_config_info &) = delete;

private:
    VkPipelineViewportStateCreateInfo viewport_info;
    VkPipelineInputAssemblyStateCreateInfo input_assembly_info;
    VkPipelineRasterizationStateCreateInfo raster_info;
    VkPipelineMultisampleStateCreateInfo multisample_info;
    VkPipelineColorBlendAttachmentState color_blend_attachment;
    VkPipelineColorBlendStateCreateInfo color_blend_info;
    VkPipelineDepthStencilStateCreateInfo depth_stencil_info;
    std::vector<VkDynamicState> dynamic_state_enables;
    VkPipelineDynamicStateCreateInfo dynamic_state_info;
    VkPipelineLayout pipeline_layout = nullptr;
    VkRenderPass render_pass = nullptr;
    uint32_t subpass = 0;

    friend class vk_pipeline;
    friend class vk_pipeline_manager;
};

class vk_pipeline {
public:
    vk_pipeline(device *dev,
            pipeline_config_info *info,
            const std::string &vert_path,
            const std::string &frag_path);
    virtual ~vk_pipeline();

    vk_pipeline(const pipeline &p) = delete;
    vk_pipeline &operator=(const vk_pipeline &p) = delete;

    void bind(vk_command_buffer *cmd_buffer);

    static void default_config_info(vk_pipeline_config_info &info);

private:
    void create_pipeline(vk_pipeline_config_info *info,
            const std::string &vert_path,
            const std::string &frag_path);

    static std::vector<VkVertexInputBindingDescription> get_binding_descriptions();
    static std::vector<VkVertexInputAttributeDescription> get_attribute_descriptions();

    vk_device *device_;
    vk_shader *shader_;
    VkPipeline vk_pipeline_;

    friend class vk_shader;
};

}

#endif //GLW_VK_PIPELINE_HPP
