//
// Created by Garrett on 3/29/22.
//

#include "vk_pipeline.hpp"
#include "../../ext/glad/vulkan.h"
#include "../../drawable/vertex.hpp"
#include "../../util/exception.hpp"

namespace glw {

vk_pipeline_config_info::vk_pipeline_config_info()
{
    this->viewport_info = {};
    this->input_assembly_info = {};
    this->raster_info = {};
    this->multisample_info = {};
    this->color_blend_attachment = {};
    this->color_blend_info = {};
    this->depth_stencil_info = {};
    this->dynamic_state_enables = {};
    this->dynamic_state_info = {};
    this->pipeline_layout = {};
    this->render_pass = {};
    this->subpass = -1;
}

vk_pipeline::vk_pipeline(
        pipeline_config_info *info,
        const std::string &vert_path,
        const std::string &frag_path
)
{
    if (!(this->device_ = vk_device::get()))
        throw vulkan_load_ex("a vulkan device is needed to create a vulkan pipeline.");

    this->create_pipeline(dynamic_cast<vk_pipeline_config_info *>(info), vert_path, frag_path);
}

vk_pipeline::~vk_pipeline()
{
    delete this->shader_;
    vkDestroyPipeline(this->device_->device_, this->vk_pipeline_, nullptr);
}

void vk_pipeline::default_config_info(vk_pipeline_config_info &info)
{
    info.viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.viewport_info.viewportCount = 1;
    info.viewport_info.pViewports = nullptr;
    info.viewport_info.scissorCount = 1;
    info.viewport_info.pScissors = nullptr;

    info.input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.input_assembly_info.primitiveRestartEnable = VK_FALSE;

    info.raster_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.raster_info.depthClampEnable = VK_FALSE;
    info.raster_info.rasterizerDiscardEnable = VK_FALSE;
    info.raster_info.polygonMode = VK_POLYGON_MODE_FILL;
    info.raster_info.cullMode = VK_CULL_MODE_NONE;
    info.raster_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    info.raster_info.depthBiasEnable = VK_FALSE;
    info.raster_info.depthBiasConstantFactor = 0.f;
    info.raster_info.depthBiasClamp = 0.f;
    info.raster_info.depthBiasSlopeFactor = 0.f;
    info.raster_info.lineWidth = 1.f;

    info.multisample_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    info.multisample_info.sampleShadingEnable = VK_FALSE;
    info.multisample_info.minSampleShading = 1.f;
    info.multisample_info.pSampleMask = nullptr;
    info.multisample_info.alphaToCoverageEnable = VK_FALSE;
    info.multisample_info.alphaToOneEnable = VK_FALSE;

    info.color_blend_attachment.blendEnable = VK_FALSE;
    info.color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    info.color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    info.color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    info.color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    info.color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    info.color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
    info.color_blend_attachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT |
                    VK_COLOR_COMPONENT_G_BIT |
                    VK_COLOR_COMPONENT_B_BIT |
                    VK_COLOR_COMPONENT_A_BIT;

    info.color_blend_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info.color_blend_info.logicOpEnable = VK_FALSE;
    info.color_blend_info.logicOp = VK_LOGIC_OP_COPY;
    info.color_blend_info.attachmentCount = 1;
    info.color_blend_info.pAttachments = &info.color_blend_attachment;
    info.color_blend_info.blendConstants[0] = 0.f;
    info.color_blend_info.blendConstants[1] = 0.f;
    info.color_blend_info.blendConstants[2] = 0.f;
    info.color_blend_info.blendConstants[3] = 0.f;

    info.depth_stencil_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    info.depth_stencil_info.depthTestEnable = VK_TRUE;
    info.depth_stencil_info.depthWriteEnable = VK_TRUE;
    info.depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS;
    info.depth_stencil_info.depthBoundsTestEnable = VK_FALSE;
    info.depth_stencil_info.stencilTestEnable = VK_FALSE;
    info.depth_stencil_info.front = {};
    info.depth_stencil_info.back = {};
    info.depth_stencil_info.minDepthBounds = 0.f;
    info.depth_stencil_info.maxDepthBounds = 1.f;

    info.dynamic_state_enables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

    info.dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    info.dynamic_state_info.pDynamicStates = info.dynamic_state_enables.data();
    info.dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(info.dynamic_state_enables.size());
    info.dynamic_state_info.flags = 0;
}

void vk_pipeline::bind(vk_command_buffer *cmd_buffer)
{
    vkCmdBindPipeline(
            static_cast<VkCommandBuffer>(cmd_buffer->raw()),
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            this->vk_pipeline_);
}

void vk_pipeline::create_pipeline(vk_pipeline_config_info *info,
        const std::string &vert_path,
        const std::string &frag_path)
{
    this->shader_ = dynamic_cast<vk_shader *>(shader::create(
            "pipeline shader",
            vert_path,
            frag_path
    ));

    VkPipelineShaderStageCreateInfo shader_stages[2]{};

    shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[0].flags = 0;
    shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stages[0].module = this->shader_->vert_sm_;
    shader_stages[0].pName = "main";

    shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stages[1].flags = 0;
    shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stages[1].module = this->shader_->frag_sm_;
    shader_stages[1].pName = "main";

    auto binding_descriptions = vk_pipeline::get_binding_descriptions();
    auto attribute_descriptions = vk_pipeline::get_attribute_descriptions();
    VkPipelineVertexInputStateCreateInfo vertex_input_info{};

    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size());
    vertex_input_info.pVertexBindingDescriptions = binding_descriptions.data();
    vertex_input_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();

    VkGraphicsPipelineCreateInfo pipeline_info{};

    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &info->input_assembly_info;
    pipeline_info.pViewportState = &info->viewport_info;
    pipeline_info.pRasterizationState = &info->raster_info;
    pipeline_info.pMultisampleState = &info->multisample_info;
    pipeline_info.pDepthStencilState = &info->depth_stencil_info;
    pipeline_info.pColorBlendState = &info->color_blend_info;
    pipeline_info.pDynamicState = &info->dynamic_state_info;
    pipeline_info.layout = info->pipeline_layout;
    pipeline_info.renderPass = info->render_pass;
    pipeline_info.subpass = info->subpass;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_info.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(this->device_->device_,
            VK_NULL_HANDLE,
            1,
            &pipeline_info,
            nullptr,
            &this->vk_pipeline_) != VK_SUCCESS)
        throw vulkan_load_ex("failed to create graphics pipeline");
}

std::vector<VkVertexInputBindingDescription> vk_pipeline::get_binding_descriptions()
{
    std::vector<VkVertexInputBindingDescription> binding_descriptions(1);

    binding_descriptions[0].binding = 0;
    binding_descriptions[0].stride = sizeof(vertex);
    binding_descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return binding_descriptions;
}

std::vector<VkVertexInputAttributeDescription> vk_pipeline::get_attribute_descriptions()
{
    std::vector<VkVertexInputAttributeDescription> attribute_descriptions{};

    attribute_descriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, position)});
    attribute_descriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, color)});
    attribute_descriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(vertex, normal)});
    attribute_descriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(vertex, uv)});

    return attribute_descriptions;
}

}