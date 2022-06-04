//
// Created by Garrett on 10/27/2021.
//

#include "gl_buffer.hpp"
#include "../../ext/glad/gl.h"

namespace glw {

gl_vertex_buffer::gl_vertex_buffer(const std::vector<vertex> &vertices)
{

    auto data = vertex::to_floats(vertices);

    glGenBuffers(1, &this->gl_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
    glBufferData(
            GL_ARRAY_BUFFER,
            data.size() * sizeof(float),
            (void *) data.data(),
            GL_STATIC_DRAW
    );

    this->vertices_ = vertices;
}

gl_vertex_buffer::~gl_vertex_buffer()
{
    glDeleteBuffers(GL_ARRAY_BUFFER, &this->gl_id);
}

void gl_vertex_buffer::bind(command_buffer *cmds) const
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
}

void gl_vertex_buffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void gl_vertex_buffer::write_data(void *vertices, uint64_t size, uint64_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
    glBufferData(
            GL_ARRAY_BUFFER,
            size * sizeof(float),
            reinterpret_cast<const void *>(vertices),
            GL_STATIC_DRAW
    );

    this->vertices_.clear();
    this->vertices_.reserve(size);

    auto v = static_cast<vertex *>(vertices);

    for (auto i = 0; i < size; i++) {
        this->vertices_.push_back(v[i]);
    }
}

gl_index_buffer::gl_index_buffer(const std::vector<uint32_t> &indices)
{
    glGenBuffers(1, &this->gl_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(uint32_t),
            reinterpret_cast<const void *>(indices.data()),
            GL_STATIC_DRAW);

    this->count_ = indices.size();
}

gl_index_buffer::~gl_index_buffer()
{
    glDeleteBuffers(1, &this->gl_id);
}

void gl_index_buffer::bind(command_buffer *cmds) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
}

void gl_index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void gl_index_buffer::write_data(void *indices, uint64_t size, uint64_t offset)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            size * sizeof(uint32_t),
            reinterpret_cast<const void *>(indices),
            GL_STATIC_DRAW
    );

    this->count_ = size;
}

}