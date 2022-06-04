//
// Created by Garrett on 12/1/2021.
//

#include <algorithm>

#include "scene.hpp"

namespace glw {

static void _update_cb(frame_info &info)
{
    for (auto n: info.nodes) {
        n->draw();
    }
}

scene::scene()
{
    this->nodes_ = std::vector<drawable *>();
    this->update_ = _update_cb;
    this->renderer_ = nullptr;
    this->prev_time = std::chrono::high_resolution_clock::now();
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
    this->renderer_->begin_swap_chain_render_pass(cmd_buffer);

    if (this->update_) {
        auto new_time = std::chrono::high_resolution_clock::now();

        frame_info info = {
                this->renderer_->get_frame_index(),
                std::chrono::duration<float, std::chrono::seconds::period>(new_time - prev_time).count(),
                cmd_buffer,
                this->renderer_->get_descriptor_set(),
                this->nodes_
        };

        this->prev_time = std::chrono::high_resolution_clock::now();
        this->update_(info);
    }

    this->renderer_->end_swap_chain_render_pass(cmd_buffer);
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
