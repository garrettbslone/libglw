//
// Created by Garrett on 10/30/2021.
//

#include "gl_context.hpp"

#include "../../ext/glad/gl.h"
#include "../../ext/glfw/include/GLFW/glfw3.h"


#include "../../util/exception.hpp"

namespace glw {

gl_context::gl_context(GLFWwindow *window)
{
    this->window = window;
}

void gl_context::init()
{
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGL(glfwGetProcAddress))
        throw gl_load_ex("Failed to initialize OpenGL context. GLAD load failed.");
}

void gl_context::swap_buffers()
{
    glfwSwapBuffers(this->window);
}

}
