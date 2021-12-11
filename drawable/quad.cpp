//
// Created by Garrett on 12/6/2021.
//

#include "quad.hpp"

#include "../ext/glad/gl.h"
#include "../graphics/opengl/topology.hpp"

namespace glw {

quad::quad()
{
    std::vector<float> p = {
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
    };
    std::vector<float> uv = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    std::vector<float> n = {
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
    };

    this->positions_ = vertex_buffer::create(p.data(), p.size(), 3);
    this->uvs_ = vertex_buffer::create(uv.data(), uv.size(), 2);
    this->normals_ = vertex_buffer::create(n.data(), n.size(), 3);

    this->ib_ = nullptr;
    this->va_ = vertex_array::create();

    this->va_->add_vertex_buffer(this->positions_);
    this->va_->add_vertex_buffer(this->uvs_);
    this->va_->add_vertex_buffer(this->normals_);

    this->topology_ = topology::TRIANGLE_STRIP;
    this->model_ = glm::mat4(1.f);
    this->shader_ = nullptr;
}

quad::~quad()
{
    this->positions_->unbind();
    this->uvs_->unbind();
    this->va_->unbind();

    delete this->positions_;
    delete this->uvs_;
    delete this->va_;
}

void quad::draw()
{
    this->va_->bind();

    if (this->ib_ && this->ib_->get_count() > 0)
        glDrawElements(gl_topology::get_gl_topology(this->topology_),
                       (GLint) this->ib_->get_count(),
                       GL_UNSIGNED_INT,
                       nullptr);
    else
        glDrawArrays(gl_topology::get_gl_topology(this->topology_),
                     0,
                     (GLint) this->positions_->get_data_size());
}
}
