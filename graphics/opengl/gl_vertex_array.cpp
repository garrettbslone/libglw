//
// Created by Garrett on 10/27/2021.
//

#include <iostream>

#include "gl_vertex_array.hpp"
#include "../../ext/glad/gl.h"

namespace glw {

gl_vertex_array::gl_vertex_array()
{
    glGenVertexArrays(1, &this->gl_id);
}

gl_vertex_array::~gl_vertex_array()
{
    glDeleteVertexArrays(1, &this->gl_id);
}

void gl_vertex_array::bind() const
{
    glBindVertexArray(this->gl_id);
}

void gl_vertex_array::unbind() const
{
    glBindVertexArray(0);
}

void gl_vertex_array::add_vertex_buffer(vertex_buffer *buff)
{
    glBindVertexArray(this->gl_id);

    buff->bind();

    glEnableVertexAttribArray(this->v_buff_index);
    glVertexAttribPointer(this->v_buff_index++,
                          buff->get_vertex_size(),
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          reinterpret_cast<GLvoid *>(this->offset_));

    this->offset_ += buff->get_vertex_size() * sizeof(float);
    this->v_buffs.push_back(buff);
}

void gl_vertex_array::set_index_buffer(index_buffer *buff)
{
    glBindVertexArray(this->gl_id);

    this->i_buff = buff;
    this->i_buff->bind();
}

}
