//
// Created by Garrett on 3/18/22.
//

#ifndef GLW_RENDERER_HPP
#define GLW_RENDERER_HPP

#include "../core/window.hpp"
#include "buffer.hpp"
#include "graphics_api.hpp"
#include "swap_chain.hpp"

namespace glw {

class renderer {
public:
    virtual ~renderer() = default;

    renderer(const renderer &) = delete;
    renderer &operator=(const renderer &) = delete;

    bool is_frame_in_progress() const { return this->is_frame_started_; }

    inline int get_frame_index() const
    {
        return this->is_frame_started_ ? this->current_frame_index_ : -1;
    }

    virtual void init() = 0;

    virtual float get_aspect_ratio() const  = 0;

    virtual buffer *begin_frame() = 0;
    virtual void end_frame() = 0;

    virtual void begin_render_pass(buffer *cmd_buffer) = 0;
    virtual void end_render_pass(buffer *cmd_buffer) = 0;

    static renderer *create(window *w, device *d, graphics_api api);

protected:
    renderer(window *w, device *d);

    virtual void create_command_buffers();
    virtual void free_command_buffers();
    virtual void recreate_swap_chain();

    window *window_;
    device *device_;
    swap_chain *swap_chain_;
    std::vector<buffer *> command_buffers_{};

    uint32_t current_image_index_{0};
    int current_frame_index_{0};
    bool is_frame_started_{false};
};

}

#endif //GLW_RENDERER_HPP
