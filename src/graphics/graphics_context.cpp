//
// Created by Garrett on 10/30/2021.
//

#include "graphics_context.hpp"
#include "opengl/gl_context.hpp"

struct GLFWwindow;

namespace glw {

graphics_context *graphics_context::create(void *window)
{
    return new gl_context(static_cast<GLFWwindow *>(window));
}

}
