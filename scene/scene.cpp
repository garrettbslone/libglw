//
// Created by Garrett on 12/1/2021.
//

#include <algorithm>
#include <iostream>

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

void scene::attach()
{
    std::cout << "scene attached\n";
}

void scene::update()
{
    if (this->update_)
        this->update_(this->nodes_);
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
