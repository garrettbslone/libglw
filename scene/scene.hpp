//
// Created by Garrett on 12/1/2021.
//

#ifndef GLW_SCENE_HPP
#define GLW_SCENE_HPP

#include <vector>
#include <functional>

#include "../drawable/drawable.hpp"
#include "../graphics/renderer.hpp"

namespace glw {

using update_cb = std::function<void(std::vector<drawable *>)>;

class scene {
public:
    scene();
    virtual ~scene();

    void push_node(drawable *d);
    void pop_node(drawable *d);

    void attach(renderer *r);
    void update();

    void on_update(update_cb cb);

    std::vector<drawable *> get_nodes();

private:
    renderer *renderer_;
    std::vector<drawable *> nodes_;
    update_cb update_;
};

}

#endif //GLW_SCENE_HPP
