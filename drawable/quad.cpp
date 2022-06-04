//
// Created by Garrett on 12/6/2021.
//

#include "quad.hpp"

#include "../ext/glad/gl.h"
#include "../graphics/opengl/topology.hpp"

namespace glw {

quad::quad()
{
    std::vector<vertex> verts = {
            {{-1.0f, 1.0f, 0.0f}, {0.f, 0.f, 1.f}, {0.0, 0.0, 1.0}, {0.0f, 1.0f}},
            {{-1.0f, -1.0f, 0.0f}, {0.f, 0.f, 1.f}, {0.0, 0.0, 1.0}, {0.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.f, 0.f, 1.f}, {0.0, 0.0, 1.0}, {1.0f, 1.0f}},
            {{1.0f, -1.0f, 0.0f}, {0.f, 0.f, 1.f}, {0.0, 0.0, 1.0}, {1.0f, 0.0f}}
    };
    this->vertices_ = vertex_buffer::create(verts);

    std::vector<uint32_t> inds = {
            0, 1, 2,
            1, 3, 2
    };

    this->ib_ = index_buffer::create(inds);

    this->va_ = vertex_array::create();
    this->va_->set_index_buffer(this->ib_);
    this->va_->add_vertex_buffer(this->vertices_);

    this->topology_ = topology::TRIANGLES;
    this->model_ = glm::mat4(1.f);
    this->shader_ = nullptr;
}

quad::~quad()
{
    this->vertices_->unbind();
    this->ib_->unbind();
    this->va_->unbind();

    delete this->vertices_;
    delete this->ib_;
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
                     (GLint) this->vertices_->get_data_size());
}
}
