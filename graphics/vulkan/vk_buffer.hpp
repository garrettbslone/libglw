//
// Created by Garrett on 3/30/22.
//

#ifndef GLW_VK_BUFFER_HPP
#define GLW_VK_BUFFER_HPP

#include "../buffer.hpp"
#include "vk_device.hpp"

namespace glw {

class vk_buffer : public buffer {
public:
    vk_buffer(vk_device *dev,
        VkDeviceSize instance_size,
        uint32_t instance_count,
        VkBufferUsageFlags usage_flags,
        VkMemoryPropertyFlags memory_property_flags,
        VkDeviceSize min_offset_alignment = 1);
    ~vk_buffer() override;

    vk_buffer(const vk_buffer&) = delete;
    vk_buffer& operator=(const vk_buffer&) = delete;

    virtual void bind(command_buffer *cmds) const override;
    void unbind() const override;
    void write_data(void *data, uint64_t size, uint64_t offset) override;

protected:
    VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    void unmap();

    void write_to_buffer(void *data, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkDescriptorBufferInfo descriptor_info(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
    VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

    void write_to_index(void *data, int index);
    VkResult flush_index(int index);
    VkDescriptorBufferInfo descriptor_info_for_index(int index);
    VkResult invalidate_index(int index);

    VkBuffer get_buffer() const { return this->buffer_; }
    void* get_mapped_memory() const { return this->mapped_; }
    uint32_t get_instance_count() const { return this->instance_count_; }
    VkDeviceSize get_instance_size() const { return this->instance_size_; }
    VkDeviceSize get_alignment_size() const { return this->alignment_size_; }
    VkBufferUsageFlags get_usage_flags() const { return this->usage_flags_; }
    VkMemoryPropertyFlags get_memory_property_flags() const { return this->memory_property_flags_; }
    VkDeviceSize get_buffer_size() const { return this->buffer_size_; }

    static VkDeviceSize get_alignment(VkDeviceSize instance_size, VkDeviceSize min_offset_alignment);

    vk_device *device_;
    void *mapped_ = nullptr;
    VkBuffer buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory memory_ = VK_NULL_HANDLE;

    VkDeviceSize buffer_size_;
    uint32_t instance_count_;
    VkDeviceSize instance_size_;
    VkDeviceSize alignment_size_;
    VkBufferUsageFlags usage_flags_;
    VkMemoryPropertyFlags memory_property_flags_;

    friend class vk_descriptor_set_layout;
    friend class vk_descriptor_pool;
    friend class vk_descriptor_writer;
    friend class vk_framebuffer;
    friend class vk_pipeline;
    friend class vk_renderer;
    friend class vk_shader;
    friend class vk_swap_chain;
};

class vk_vertex_buffer : public vk_buffer, vertex_buffer {
public:
    vk_vertex_buffer(vk_device *dev,
        VkDeviceSize instance_size,
        uint32_t instance_count,
        VkBufferUsageFlags usage_flags,
        VkMemoryPropertyFlags memory_property_flags,
        VkDeviceSize min_offset_alignment = 1);
    ~vk_vertex_buffer() override;
    void bind(struct command_buffer *cmds) const override;
    void unbind() const override;
    void write_data(void *vertices, uint64_t size, uint64_t offset) override;

private:
    friend class vk_descriptor_set_layout;
    friend class vk_descriptor_pool;
    friend class vk_descriptor_writer;
    friend class vk_framebuffer;
    friend class vk_pipeline;
    friend class vk_renderer;
    friend class vk_shader;
    friend class vk_swap_chain;
};

class vk_index_buffer : public vk_buffer, index_buffer {
public:
    vk_index_buffer(vk_device *dev,
        VkDeviceSize instance_size,
        uint32_t instance_count,
        VkBufferUsageFlags usage_flags,
        VkMemoryPropertyFlags memory_property_flags,
        VkDeviceSize min_offset_alignment = 1);

    void write_data(void *vertices, uint64_t size, uint64_t offset) override;
    ~vk_index_buffer() override;
    void bind(command_buffer *cmds) const override;
    void unbind() const override;

private:
    friend class vk_descriptor_set_layout;
    friend class vk_descriptor_pool;
    friend class vk_descriptor_writer;
    friend class vk_framebuffer;
    friend class vk_pipeline;
    friend class vk_renderer;
    friend class vk_shader;
    friend class vk_swap_chain;
};

class vk_command_buffer : public command_buffer {
public:
    vk_command_buffer();
    ~vk_command_buffer() override;

    void *raw() override;

private:
    VkCommandBuffer buffer_{};
};

}

#endif //GLW_VK_BUFFER_HPP
