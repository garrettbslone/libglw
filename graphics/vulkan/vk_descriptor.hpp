//
// Created by Garrett on 3/30/22.
//

#ifndef GLW_VK_DESCRIPTOR_HPP
#define GLW_VK_DESCRIPTOR_HPP

#include "vk_device.hpp"

#include <unordered_map>
#include <vector>

namespace glw {

class vk_descriptor_set_layout {
public:
    class builder {
    public:
        explicit builder() : device_{vk_device::get()} {}

        builder &add_binding(
                uint32_t binding,
                VkDescriptorType descriptor_type,
                VkShaderStageFlags stage_flags,
                uint32_t count = 1);
        vk_descriptor_set_layout *build() const;

    private:
        vk_device *device_;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings_{};
    };

    vk_descriptor_set_layout(
            const std::unordered_map<uint32_t,
            VkDescriptorSetLayoutBinding>& bindings
    );
    ~vk_descriptor_set_layout();

    vk_descriptor_set_layout(const vk_descriptor_set_layout &) = delete;
    vk_descriptor_set_layout &operator=(const vk_descriptor_set_layout &) = delete;

private:
    inline VkDescriptorSetLayout get_descriptor_set_layout() const
    {
        return this->descriptor_set_layout_;
    }

    vk_device *device_;
    VkDescriptorSetLayout descriptor_set_layout_;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings_;

    friend class vk_descriptor_writer;
    friend class vk_pipeline_manager;
};

class vk_descriptor_pool {
public:
    class builder {
    public:
        builder() : device_{vk_device::get()} {}

        builder &add_pool_size(VkDescriptorType descriptor_type, uint32_t count);
        builder &set_pool_flags(VkDescriptorPoolCreateFlags flags);
        builder &set_max_sets(uint32_t count);
        vk_descriptor_pool *build() const;

    private:
        vk_device *device_;
        std::vector<VkDescriptorPoolSize> pool_sizes_{};
        uint32_t max_sets_ = 1000;
        VkDescriptorPoolCreateFlags pool_flags_ = 0;
    };

    vk_descriptor_pool(
            uint32_t max_sets,
            VkDescriptorPoolCreateFlags pool_flags,
            const std::vector<VkDescriptorPoolSize> &pool_sizes
    );
    ~vk_descriptor_pool();

    vk_descriptor_pool(const vk_descriptor_pool &) = delete;
    vk_descriptor_pool &operator=(const vk_descriptor_pool &) = delete;

    bool allocate_descriptor_set(VkDescriptorSetLayout descriptor_set_layout,
            VkDescriptorSet &descriptor) const;

    void free_descriptors(std::vector<VkDescriptorSet> &descriptors) const;

    void reset_pool();

private:
    vk_device *device_;
    VkDescriptorPool descriptor_pool_;

    friend class vk_descriptor_writer;
};

class vk_descriptor_writer {
public:
    vk_descriptor_writer(vk_descriptor_set_layout &set_layout, vk_descriptor_pool &pool);

    vk_descriptor_writer &write_buffer(uint32_t binding,
            VkDescriptorBufferInfo *buffer_info);
    vk_descriptor_writer &write_image(uint32_t binding,
            VkDescriptorImageInfo *image_info);

    bool build(VkDescriptorSet &set);
    void overwrite(VkDescriptorSet &set);

private:
    vk_descriptor_set_layout &set_layout_;
    vk_descriptor_pool &pool_;
    std::vector<VkWriteDescriptorSet> writes_;
};

}

#endif //GLW_VK_DESCRIPTOR_HPP
