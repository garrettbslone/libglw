//
// Created by Garrett on 10/30/2021.
//

#include "gl_renderer.hpp"

#include "../../ext/glad/gl.h"
#include "../../ext/glfw/include/GLFW/glfw3.h"

namespace glw {

gl_renderer::gl_renderer(window *w, device *d)
    : renderer(w, d)
{
}

void gl_renderer::init()
{
    glfwMakeContextCurrent(this->window_->get_g_window());
}

float gl_renderer::get_aspect_ratio() const
{
    return static_cast<float>(this->window_->get_aspect_ratio());
}

buffer *gl_renderer::begin_frame()
{
    return nullptr;
}

/*
 * Swap the front and back buffers. This is basically the opengl swap chain.
 * but it is handled behind the scenes by the opengl context.
 */
void gl_renderer::end_frame()
{
    glfwSwapBuffers(this->window_->get_g_window());
}

void gl_renderer::begin_swap_chain_render_pass(buffer *cmd_buffer)
{

}

void gl_renderer::end_swap_chain_render_pass(buffer *cmd_buffer)
{

}

}
