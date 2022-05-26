//
// Created by Garrett on 3/30/22.
//

#include "vk_buffer.hpp"

#include <cassert>
#include <cstring>

namespace glw {

vk_buffer::vk_buffer(
        VkDeviceSize instance_size,
        uint32_t instance_count,
        VkBufferUsageFlags usage_flags,
        VkMemoryPropertyFlags memory_property_flags,
        VkDeviceSize min_offset_alignment)
        : instance_size_{instance_size},
          instance_count_{instance_count},
          usage_flags_{usage_flags},
          memory_property_flags_{memory_property_flags} {
    this->device_ = vk_device::get(),
    this->alignment_size_ = get_alignment(instance_size, min_offset_alignment);
    this->buffer_size_ = this->alignment_size_ * instance_count;

    this->device_->create_buffer(this->buffer_size_,
            usage_flags,
            memory_property_flags,
            this->buffer_,
            this->memory_);
}

vk_buffer::~vk_buffer() {
    unmap();
    vkDestroyBuffer(this->device_->device_, this->buffer_, nullptr);
    vkFreeMemory(this->device_->device_, this->memory_, nullptr);
}

/**
 * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
 *
 * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
 * buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the buffer mapping call
 */
VkResult vk_buffer::map(VkDeviceSize size, VkDeviceSize offset) {
    assert(this->buffer_ && this->memory_ && "Called map on buffer before create");
    return vkMapMemory(this->device_->device_, this->memory_, offset, size, 0, &this->mapped_);
}

/**
 * Unmap a mapped memory range
 *
 * @note Does not return a result as vkUnmapMemory can't fail
 */
void vk_buffer::unmap() {
    if (this->mapped_) {
        vkUnmapMemory(this->device_->device_, this->memory_);
        this->mapped_ = nullptr;
    }
}

/**
 * Copies the specified data to the mapped buffer. Default value writes whole buffer range
 *
 * @param data Pointer to the data to copy
 * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
 * range.
 * @param offset (Optional) Byte offset from beginning of mapped region
 *
 */
void vk_buffer::write_to_buffer(void *data, VkDeviceSize size, VkDeviceSize offset) {
    assert(this->mapped_ && "Cannot copy to unmapped buffer");

    if (size == VK_WHOLE_SIZE) {
        memcpy(this->mapped_, data, this->buffer_size_);
    } else {
        char *memOffset = reinterpret_cast<char *>(this->mapped_);
        memOffset += offset;
        memcpy(memOffset, data, size);
    }
}

/**
 * Flush a memory range of the buffer to make it visible to the device
 *
 * @note Only required for non-coherent memory
 *
 * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
 * complete buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the flush call
 */
VkResult vk_buffer::flush(VkDeviceSize size, VkDeviceSize offset) {
    VkMappedMemoryRange mapped_range = {};

    mapped_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mapped_range.memory = this->memory_;
    mapped_range.offset = offset;
    mapped_range.size = size;

    return vkFlushMappedMemoryRanges(this->device_->device_, 1, &mapped_range);
}

/**
 * Invalidate a memory range of the buffer to make it visible to the host
 *
 * @note Only required for non-coherent memory
 *
 * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
 * the complete buffer range.
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkResult of the invalidate call
 */
VkResult vk_buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
    VkMappedMemoryRange mapped_range = {};

    mapped_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    mapped_range.memory = this->memory_;
    mapped_range.offset = offset;
    mapped_range.size = size;

    return vkInvalidateMappedMemoryRanges(this->device_->device_, 1, &mapped_range);
}

/**
 * Create a buffer info descriptor
 *
 * @param size (Optional) Size of the memory range of the descriptor
 * @param offset (Optional) Byte offset from beginning
 *
 * @return VkDescriptorBufferInfo of specified offset and range
 */
VkDescriptorBufferInfo vk_buffer::descriptor_info(VkDeviceSize size, VkDeviceSize offset) {
    return VkDescriptorBufferInfo{this->buffer_, offset,size};
}

/**
 * Copies "instance_size_" bytes of data to the mapped buffer at an offset of index * alignment_size_
 *
 * @param data Pointer to the data to copy
 * @param index Used in offset calculation
 *
 */
void vk_buffer::write_to_index(void *data, int index) {
    write_to_buffer(data, this->instance_size_, index * this->alignment_size_);
}

/**
 *  Flush the memory range at index * alignment_size_ of the buffer to make it visible to the device
 *
 * @param index Used in offset calculation
 *
 */
VkResult vk_buffer::flush_index(int index)
{
    return flush(this->alignment_size_, index * this->alignment_size_);
}

/**
 * Create a buffer info descriptor
 *
 * @param index Specifies the region given by index * alignment_size_
 *
 * @return VkDescriptorBufferInfo for instance at index
 */
VkDescriptorBufferInfo vk_buffer::descriptor_info_for_index(int index) {
    return descriptor_info(this->alignment_size_, index * this->alignment_size_);
}

/**
 * Invalidate a memory range of the buffer to make it visible to the host
 *
 * @note Only required for non-coherent memory
 *
 * @param index Specifies the region to invalidate: index * alignment_size_
 *
 * @return VkResult of the invalidate call
 */
VkResult vk_buffer::invalidate_index(int index) {
    return invalidate(this->alignment_size_, index * this->alignment_size_);
}


/**
 * Returns the minimum instance size required to be compatible with devices min_offset_alignment
 *
 * @param instance_size The size of an instance
 * @param min_offset_alignment The minimum required alignment, in bytes, for the offset member (eg
 * minUniformBufferOffsetAlignment)
 *
 * @return VkResult of the buffer mapping call
 */
VkDeviceSize vk_buffer::get_alignment(VkDeviceSize instance_size, VkDeviceSize min_offset_alignment) {
    if (min_offset_alignment > 0)
        return (instance_size + min_offset_alignment - 1) & ~(min_offset_alignment - 1);

    return instance_size;
}

void vk_buffer::bind(command_buffer *cmds) const
{

}

void vk_buffer::unbind() const
{

}

void vk_buffer::write_data(void *data, uint64_t size, uint64_t offset)
{
    if (size <= 0)
        size = VK_WHOLE_SIZE;
    if (offset < 0)
        offset = 0;
    this->write_to_buffer(data, size, offset);
}

vk_vertex_buffer::vk_vertex_buffer(
        const std::vector<vertex> &vertices,
        VkDeviceSize instance_size,
        uint32_t instance_count,
        VkBufferUsageFlags usage_flags,
        VkMemoryPropertyFlags memory_property_flags,
        VkDeviceSize min_offset_alignment)
        : vk_buffer(
            instance_size,
            instance_count,
            usage_flags,
            memory_property_flags,
            min_offset_alignment)
{
    this->map();
    this->write_to_buffer(static_cast<void *>(vertex::to_floats(vertices).data()));
}

void vk_vertex_buffer::bind(struct command_buffer *cmds) const
{
    VkBuffer buffers[] = {this->buffer_};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(
            static_cast<VkCommandBuffer>(cmds->raw()),
            0,
            1,
            buffers,
            offsets);
}

void vk_vertex_buffer::unbind() const
{
    vk_buffer::unbind();
}

void vk_vertex_buffer::write_data(void *vertices, uint64_t size, uint64_t offset)
{
    if (size <= 0)
        size = VK_WHOLE_SIZE;
    if (offset < 0)
        offset = 0;
    this->write_to_buffer(vertices, size, offset);
}

vk_vertex_buffer::~vk_vertex_buffer()
{
    this->unmap();
    vkDestroyBuffer(this->device_->device_, this->buffer_, nullptr);
    vkFreeMemory(this->device_->device_, this->memory_, nullptr);
}

vk_index_buffer::vk_index_buffer(
        const std::vector<uint32_t> &indices,
        VkDeviceSize instance_size,
        uint32_t instance_count,
        VkBufferUsageFlags usage_flags,
        VkMemoryPropertyFlags memory_property_flags,
        VkDeviceSize min_offset_alignment)
        : vk_buffer(
            instance_size,
            instance_count,
            usage_flags,
            memory_property_flags,
            min_offset_alignment)
{
    this->map();
    this->write_to_buffer((void *) indices.data());
}

void vk_index_buffer::write_data(void *indices, uint64_t size, uint64_t offset)
{
    if (size <= 0)
        size = VK_WHOLE_SIZE;
    if (offset < 0)
        offset = 0;
    this->write_to_buffer(indices, size, offset);
}

vk_index_buffer::~vk_index_buffer()
{
    this->unmap();
    vkDestroyBuffer(this->device_->device_, this->buffer_, nullptr);
    vkFreeMemory(this->device_->device_, this->memory_, nullptr);
}

void vk_index_buffer::bind(command_buffer *cmds) const
{
    vkCmdBindIndexBuffer(
            static_cast<VkCommandBuffer>(cmds->raw()),
            this->buffer_,
            0,
            VK_INDEX_TYPE_UINT32);
}

void vk_index_buffer::unbind() const
{
    vk_buffer::unbind();
}

vk_command_buffer::vk_command_buffer()
= default;

vk_command_buffer::~vk_command_buffer()
= default;

void *vk_command_buffer::raw()
{
    return static_cast<void *>(this->buffer_);
}

}
