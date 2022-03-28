//
// Created by Garrett on 10/30/2021.
//

#include "gl_renderer.hpp"

#include "../../ext/glad/gl.h"
#include "../../ext/glfw/include/GLFW/glfw3.h"

#include "../../util/exception.hpp"

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

void gl_renderer::end_frame()
{
    glfwSwapBuffers(this->window_->get_g_window());
}

void gl_renderer::begin_render_pass(buffer *cmd_buffer)
{

}

void gl_renderer::end_render_pass(buffer *cmd_buffer)
{

}

}
