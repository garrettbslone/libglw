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

buffer *buffer::create(
        uint64_t size,
        uint32_t count,
        uint32_t usage,
        uint32_t memory_usage
)
{
#ifdef HAVE_VULKAN
    return api::active == API_VULKAN ?
        new vk_buffer(size, count, usage, memory_usage) : nullptr;
#else
    return nullptr;
#endif
}

vertex_buffer *vertex_buffer::create(
        const std::vector<vertex> &vertices,
        uint64_t size,
        uint32_t count,
        uint32_t usage,
        uint32_t memory_usage
)
{
#ifdef HAVE_VULKAN
    switch (api::active) {
    case API_VULKAN:
        return new vk_vertex_buffer(vertices, size, count, usage, memory_usage);
    case API_OPEN_GL:
    case API_NONE:
    default:
        return new gl_vertex_buffer(vertices);
    }
#else
    return new gl_vertex_buffer(vertices);
#endif
}

index_buffer *index_buffer::create(
        const std::vector<uint32_t> &indices,
        uint64_t size,
        uint32_t count,
        uint32_t usage,
        uint32_t memory_usage
)
{
#ifdef HAVE_VULKAN
    switch (api::active) {
    case API_VULKAN:
        return new vk_index_buffer(indices, size, count, usage, memory_usage);
    case API_OPEN_GL:
    case API_NONE:
    default:
        return new gl_index_buffer(indices);
    }
#else
    return new gl_index_buffer(indices);
#endif
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
