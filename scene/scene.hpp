//
// Created by Garrett on 12/1/2021.
//

#ifndef GLW_SCENE_HPP
#define GLW_SCENE_HPP

#include <vector>

#include "../drawable/drawable.hpp"

namespace glw {

class scene {
public:
    scene();
    virtual ~scene();

    void push_node(drawable *d);
    void pop_node(drawable *d);

    void attach();
    void update();

    std::vector<drawable *> get_nodes();

private:
    std::vector<drawable *> nodes_;
};

}

#endif //GLW_SCENE_HPP
