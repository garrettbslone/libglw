//
// Created by Garrett on 12/1/2021.
//

#include <algorithm>
#include <iostream>

#include "scene.hpp"

namespace glw {

// takes the objects buffered in update_cb and draws them
static void _update_cb(std::vector<drawable *> nodes)
{
    for (auto n: nodes) {
        n->draw();
    }
}

// constructor definition for scene object
scene::scene()
{
    this->nodes_ = std::vector<drawable *>();
    this->update_ = _update_cb;
}

// scene object destructor used to free up memory space
scene::~scene()
{
}

// push function for drawable function
void scene::push_node(drawable *d)
{
    this->nodes_.push_back(d);
}

// pop function for drawable function
void scene::pop_node(drawable *d)
{
    auto it = std::find(this->nodes_.begin(), this->nodes_.end(), d);

    if (it != this->nodes_.end()) {
        this->nodes_.erase(it);
    }
}

/*
 * Confirmation message for scene attachment. When it is attached to an app
 * this confirmation will be printed to the console.
 */
void scene::attach()
{
    std::cout << "scene attached\n";
}

// scene update setter
void scene::update()
{
    if (this->update_)
        this->update_(this->nodes_);
}

// function replacement for update function of scene object
void scene::on_update(update_cb cb)
{
    this->update_ = cb;
}

// getter for vector of drawables
std::vector<drawable *> scene::get_nodes()
{
    return this->nodes_;
}

}
