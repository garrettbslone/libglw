//
// Created by Garrett on 4/26/22.
//

#ifndef GLW_VK_PIPELINE_MANAGER_HPP
#define GLW_VK_PIPELINE_MANAGER_HPP

#include "vk_descriptor.hpp"
#include "vk_pipeline.hpp"
#include "vk_swap_chain.hpp"

#include <unordered_map>

namespace glw {

class vk_pipeline_manager {
public:
    vk_pipeline_manager(vk_descriptor_set_layout *layout, vk_swap_chain *chain);
    virtual ~vk_pipeline_manager();

    void add_pipeline(const std::string &name, vk_pipeline *p);
    vk_pipeline *create_pipeline(
            const std::string &name,
            vk_device *dev,
            pipeline_config_info *info,
            const std::string &vert_path,
            const std::string &frag_path);
    void remove_pipeline(const std::string &name);
    void destroy_pipeline(const std::string &name);

    vk_pipeline *get_pipeline(const std::string &name);

    void clear();

    inline static vk_pipeline_manager &get() { return *instance; }

private:
    struct pipeline_data {
      vk_pipeline *pipeline_;
      vk_device *device_;
      VkPipelineLayout layout_;
      bool allocated;
    };

    std::unordered_map<std::string, pipeline_data> pipelines_;
    VkDescriptorSetLayout global_set_layout_;
    VkRenderPass render_pass_;

    static vk_pipeline_manager *instance;
};

}

#endif //GLW_VK_PIPELINE_MANAGER_HPP
