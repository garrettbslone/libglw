//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_GL_RENDERER_HPP
#define GLW_GL_RENDERER_HPP

#include "../renderer.hpp"

struct GLFWwindow;

namespace glw {

class gl_renderer : public renderer {
public:
    gl_renderer(window *w, device *d);
    ~gl_renderer() override = default;

    gl_renderer(const gl_renderer &) = delete;
    gl_renderer &operator=(const gl_renderer &) = delete;

    void init() override;

    float get_aspect_ratio() const override;

    buffer *begin_frame() override;
    void end_frame() override;

    void begin_render_pass(buffer *cmd_buffer) override;
    void end_render_pass(buffer *cmd_buffer) override;

    friend class window;
};

}

#endif //GLW_GL_RENDERER_HPP
