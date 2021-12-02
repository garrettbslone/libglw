//
// Created by Garrett on 12/1/2021.
//

#include <algorithm>
#include <iostream>

#include "scene.hpp"

namespace glw {

scene::scene()
{
    this->nodes_ = std::vector<drawable *>();
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
    for (auto n: this->nodes_) {
        n->draw();
    }
}

std::vector<drawable *> scene::get_nodes()
{
    return this->nodes_;
}

}
