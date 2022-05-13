//
// Created by Garrett on 10/27/2021.
//

#include "buffer.hpp"
#include "opengl/gl_buffer.hpp"
#ifdef HAVE_VULKAN
#include "vulkan/vk_buffer.hpp"
#endif

#include "../../util/exception.hpp"

namespace glw {

buffer *buffer::create(class device *d,
        uint64_t size,
        uint32_t count,
        uint32_t usage,
        uint32_t memory_usage,
        buffer_type type)
{
#ifdef HAVE_VULKAN
    vk_device *vkd;
    if (!(vkd = dynamic_cast<vk_device *>(d)))
        throw vulkan_device_ex("A vulkan device is required to create a vulkan buffer!");

    switch (type) {
    case INDEX_BUFFER:
        return dynamic_cast<vk_buffer *>(new vk_index_buffer(
                vkd,
                size,
                count,
                usage,
                memory_usage));
    case VERTEX_BUFFER:
        return dynamic_cast<vk_buffer *>(new vk_vertex_buffer(
                vkd,
                size,
                count,
                usage,
                memory_usage));
    case GENERIC_BUFFER:
    default:
        return new vk_buffer(vkd, size, count, usage, memory_usage);
    }
#else
    return nullptr;
#endif
}

vertex_buffer *vertex_buffer::create(
        const std::vector<vertex_data> &vertices,
        unsigned int vertex_size
        )
{
    return new gl_vertex_buffer(vertices, vertex_size);
}

index_buffer *index_buffer::create(const std::vector<uint32_t> &indices)
{
    return new gl_index_buffer(indices);
}

command_buffer *command_buffer::create()
{
#ifdef HAVE_VULKAN
    return new vk_command_buffer();
#else
    return nullptr;
#endif
}

}
