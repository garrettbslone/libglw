//
// Created by Garrett on 12/7/2021.
//

#include "wormy.hpp"

#define PI glm::pi<float>()

wormy::wormy(int vertex_count, int r)
{
    std::vector<vertex> p;

    float slice = 2 * PI / vertex_count;

    for (auto i = 0; i < vertex_count; i ++) {
        float rads = i * slice;
        vertex v = {
                {r * glm::cos(rads), 0, r * glm::sin(rads)},
                {0.f, 0.f, 0.f},
                {0.f, 1.f, 0.f},
                {0.f, 0.f}
        };
        p.push_back(v);
//        p.push_back(r * glm::cos(rads));
//        p.push_back(r * glm::sin(rads));
    }

//    this->positions_ = vertex_buffer::create(p.data(), p.size(), 3);
    this->positions_ = vertex_buffer::create(p);

    this->ib_ = nullptr;
    this->va_ = vertex_array::create();

    this->va_->add_vertex_buffer(this->positions_);

    this->topology_ = TRIANGLE_FAN_TOPO;
    this->model_ = glm::mat4(1.f);
    this->shader_ = nullptr;
}

wormy::~wormy()
{

}

void wormy::draw()
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
