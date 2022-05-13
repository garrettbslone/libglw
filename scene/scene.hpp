//
// Created by Garrett on 12/1/2021.
//

#ifndef GLW_SCENE_HPP
#define GLW_SCENE_HPP

#include <chrono>
#include <vector>
#include <functional>

#include "../drawable/drawable.hpp"
#include "../graphics/renderer.hpp"

namespace glw {

struct frame_info {
    int frame_index;
    float frame_time;
    command_buffer *cmd_buffer;
    void *descriptor_set;
    std::vector<drawable *> nodes;
};

using update_cb = std::function<void(frame_info &)>;

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
    std::chrono::time_point<std::chrono::high_resolution_clock> prev_time{};
};

}

#endif //GLW_SCENE_HPP
