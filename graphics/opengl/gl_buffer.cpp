//
// Created by Garrett on 10/27/2021.
//

#include "gl_buffer.hpp"
#include "../../ext/glad/gl.h"

namespace glw {

gl_vertex_buffer::gl_vertex_buffer(uint32_t size)
{
    glGenBuffers(1, &this->gl_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

gl_vertex_buffer::gl_vertex_buffer(float *vertices, uint32_t size, uint32_t vert_size)
{
    glGenBuffers(1, &this->gl_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);

    for (auto i = 0; i < size; i++) {
        this->vertices.push_back(vertices[i]);
    }

    this->vertex_size = vert_size;
}

gl_vertex_buffer::~gl_vertex_buffer()
{
    glDeleteBuffers(GL_ARRAY_BUFFER, &this->gl_id);
}

void gl_vertex_buffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
}

void gl_vertex_buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl_vertex_buffer::set_data(const void *data, uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

    this->vertices.clear();

    auto *_data = (float *) data;

    for (auto i = 0; i < size; i++) {
        this->vertices.push_back((float) _data[i]);
    }
}

gl_index_buffer::gl_index_buffer(uint32_t *indices, uint32_t count)
{
    glGenBuffers(1, &this->gl_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    this->count = count;
}

gl_index_buffer::~gl_index_buffer()
{
    glDeleteBuffers(1, &this->gl_id);
}

void gl_index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
}

void gl_index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}