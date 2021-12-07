//
// Created by Garrett on 10/27/2021.
//

#include "buffer.hpp"
#include "opengl/gl_buffer.hpp"

namespace glw {

vertex_buffer *vertex_buffer::create(uint32_t size)
{
    return new gl_vertex_buffer(size);
}

vertex_buffer *vertex_buffer::create(float *vertices, uint32_t size, uint32_t vert_size)
{
    return new gl_vertex_buffer(vertices, size, vert_size);
}

index_buffer *index_buffer::create(uint32_t *indices, uint32_t count)
{
    return new gl_index_buffer(indices, count);
}

}
