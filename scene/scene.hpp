//
// Created by Garrett on 12/1/2021.
//

#ifndef GLW_SCENE_HPP
#define GLW_SCENE_HPP

#include <vector>
#include <functional>

#include "../drawable/drawable.hpp"

namespace glw {

// update callback function used to update the drawables in the scene
using update_cb = std::function<void(std::vector<drawable *>)>;

class scene {
// public declarations of class elements
public:
    // scene object constructor
    scene();
    // scene object destructor
    virtual ~scene();

    //push and pop functions for vector of drawables
    void push_node(drawable *d);
    void pop_node(drawable *d);

    // scene attach method
    void attach();

    // scene update setter method
    void update();

    // scene update method that serves as "Update and Render" style function
    void on_update(update_cb cb);

    // scene nodes getter
    std::vector<drawable *> get_nodes();

// declaration of private class elements
private:
    // vector of drawables
    std::vector<drawable *> nodes_;

    // update_cb object
    update_cb update_;
};

}

#endif //GLW_SCENE_HPP
