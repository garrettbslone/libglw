//
// Created by Garrett on 4/26/22.
//

#include "vk_pipeline_manager.hpp"

#include "../../util/exception.hpp"

namespace glw {

vk_pipeline_manager *vk_pipeline_manager::instance = nullptr;

vk_pipeline_manager::vk_pipeline_manager(
        vk_descriptor_set_layout *layout,
        vk_swap_chain *chain)
{
    if (instance)
        return;

    instance = this;

    this->pipelines_ = std::unordered_map<std::string, pipeline_data>();
    this->global_set_layout_ = layout->get_descriptor_set_layout();
    this->render_pass_ = chain->get_render_pass();
}

vk_pipeline_manager::~vk_pipeline_manager()
{
    this->clear();
}

void vk_pipeline_manager::add_pipeline(const std::string &name, vk_pipeline *p)
{
    if (this->pipelines_.find(name) == this->pipelines_.end()) {
        pipeline_data data = {
                p,
                nullptr,
                nullptr,
                false
        };

        this->pipelines_.emplace(name, data);
    }
}

vk_pipeline *vk_pipeline_manager::create_pipeline(
        const std::string &name,
        vk_device *dev,
        pipeline_config_info *info,
        const std::string &vert_path,
        const std::string &frag_path)
{
    if (this->pipelines_.find(name) == this->pipelines_.end()) {
        VkPushConstantRange push_constant_range{};

        push_constant_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        push_constant_range.offset = 0;
        push_constant_range.size = sizeof(push_constant_data);

        std::vector<VkDescriptorSetLayout> descriptor_set_layouts{this->global_set_layout_};

        VkPipelineLayoutCreateInfo pipeline_layout_info{};

        pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(descriptor_set_layouts.size());
        pipeline_layout_info.pSetLayouts = descriptor_set_layouts.data();
        pipeline_layout_info.pushConstantRangeCount = 1;
        pipeline_layout_info.pPushConstantRanges = &push_constant_range;

        VkPipelineLayout pipeline_layout;

        if (vkCreatePipelineLayout(
                dev->device_,
                &pipeline_layout_info,
                nullptr,
                &pipeline_layout) != VK_SUCCESS)
            return nullptr;

        vk_pipeline_config_info pipeline_config{};
        vk_pipeline::default_config_info(pipeline_config);

        pipeline_config.render_pass = this->render_pass_;
        pipeline_config.pipeline_layout = pipeline_layout;

        vk_pipeline *p = reinterpret_cast<vk_pipeline *>(pipeline::create(
                dev,
                &pipeline_config,
                vert_path,
                frag_path,
                API_VULKAN));

        pipeline_data data = {
                p,
                dev,
                pipeline_layout,
                true
        };

        this->pipelines_.emplace(name, data);
        return p;
    }

    return nullptr;
}

void vk_pipeline_manager::remove_pipeline(const std::string &name)
{
    auto it = this->pipelines_.find(name);

    if (it != this->pipelines_.end())
        this->pipelines_.erase(it);
}

void vk_pipeline_manager::destroy_pipeline(const std::string &name)
{
    auto it = this->pipelines_.find(name);

    if (it != this->pipelines_.end()) {
        delete it->second.pipeline_;
        this->pipelines_.erase(it);
    }
}

vk_pipeline *vk_pipeline_manager::get_pipeline(const std::string &name)
{
    auto it = this->pipelines_.find(name);

    if (it != this->pipelines_.end())
        return it->second.pipeline_;

    return nullptr;
}

void vk_pipeline_manager::clear()
{
    for (auto &[name, data]: this->pipelines_) {
        if (data.device_)
            vkDestroyPipelineLayout(data.device_->device_, data.layout_, nullptr);

        if (data.allocated)
            delete data.pipeline_;
    }

    this->pipelines_.clear();
}

}
