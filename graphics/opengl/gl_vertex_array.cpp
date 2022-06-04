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

    buff->bind(nullptr);


    // calculate stride from number of non-empty vertex attribute arrays
    // position + color + normal + uv
    size_t stride = (3 + 3 + 3 + 2) * sizeof(float);

    // position attribute
    size_t offset = 0;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*) offset);
    offset += 3 * sizeof(float);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*) offset);
    offset += 3 * sizeof(float);

    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*) offset);
    offset += 3 * sizeof(float);

    // uv attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*) offset);

    this->v_buffs.push_back(buff);
}

void gl_vertex_array::set_index_buffer(index_buffer *buff)
{
    glBindVertexArray(this->gl_id);

    this->i_buff = buff;
    this->i_buff->bind(nullptr);
}

}
