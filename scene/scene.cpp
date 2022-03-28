//
// Created by Garrett on 12/1/2021.
//

#include <algorithm>

#include "scene.hpp"

namespace glw {

static void _update_cb(std::vector<drawable *> nodes)
{
    for (auto n: nodes) {
        n->draw();
    }
}

scene::scene()
{
    this->nodes_ = std::vector<drawable *>();
    this->update_ = _update_cb;
    this->renderer_ = nullptr;
}

scene::~scene()
{
}

void scene::push_node(drawable *d)
{
    this->nodes_.push_back(d);
}

void scene::pop_node(drawable *d)
{
    auto it = std::find(this->nodes_.begin(), this->nodes_.end(), d);

    if (it != this->nodes_.end()) {
        this->nodes_.erase(it);
    }
}

void scene::attach(renderer *r)
{
    this->renderer_ = r;
    this->renderer_->init();
}

void scene::update()
{
    auto cmd_buffer = this->renderer_->begin_frame();
    this->renderer_->begin_render_pass(cmd_buffer);

    if (this->update_)
        this->update_(this->nodes_);

    this->renderer_->end_render_pass(cmd_buffer);
    this->renderer_->end_frame();
}

void scene::on_update(update_cb cb)
{
    this->update_ = cb;
}

std::vector<drawable *> scene::get_nodes()
{
    return this->nodes_;
}

}
