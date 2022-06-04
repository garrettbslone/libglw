//
// Created by Garrett on 10/30/2021.
//

#include "gl_renderer.hpp"

#include "../../ext/glad/gl.h"
#include "../../ext/glfw/include/GLFW/glfw3.h"

namespace glw {

gl_renderer::gl_renderer(window *w)
    : renderer(w)
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

command_buffer * gl_renderer::begin_frame()
{
    return nullptr;
}

/*
 * Swap the front and back buffers. This is basically the opengl swap chain.
 * but it is handled behind the scenes by the opengl context.
 */
void gl_renderer::end_frame()
{
    glfwSwapBuffers(this->window_->native_window_);
}

void gl_renderer::begin_swap_chain_render_pass(command_buffer *cmd_buffer)
{

}

void gl_renderer::end_swap_chain_render_pass(command_buffer *cmd_buffer)
{

}

void *gl_renderer::get_descriptor_set()
{
    return nullptr;
}

}
