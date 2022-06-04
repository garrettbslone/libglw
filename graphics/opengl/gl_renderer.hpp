//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_GL_RENDERER_HPP
#define GLW_GL_RENDERER_HPP

#include "../renderer.hpp"

namespace glw {

/*
 * An OpenGL defined renderer.
 */
class gl_renderer : public renderer {
public:
    explicit gl_renderer(window *w);
    ~gl_renderer() override = default;

    gl_renderer(const gl_renderer &) = delete;
    gl_renderer &operator=(const gl_renderer &) = delete;

    void init() override;

    float get_aspect_ratio() const override;

    command_buffer * begin_frame() override;
    void end_frame() override;

    void begin_swap_chain_render_pass(command_buffer *cmd_buffer) override;
    void end_swap_chain_render_pass(command_buffer *cmd_buffer) override;

    void *get_descriptor_set() override;
};

}

#endif //GLW_GL_RENDERER_HPP
