//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_GL_CONTEXT_HPP
#define GLW_GL_CONTEXT_HPP

#include "../graphics_context.hpp"

struct GLFWwindow;

namespace glw {

class gl_context : public graphics_context {
public:
    gl_context(GLFWwindow *window);

    void init() override;
    void swap_buffers() override;

private:
    GLFWwindow *window;
};

}

#endif //GLW_GL_CONTEXT_HPP
