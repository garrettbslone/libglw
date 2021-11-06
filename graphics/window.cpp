//
// Created by Garrett on 9/28/2021.
//

#include <cmath>

#define GLAD_GL_IMPLEMENTATION
#include "../ext/glad/gl.h"

#include "../util/exception.hpp"
#include "window.hpp"


namespace glw {

window::window()
    : window(WINDOW_MODE_MAXIMIZED, "Main Window") {}

window::window(int dim)
    : window(dim, "Main Window") {}

window::window(short mode)
    : window(mode, "Main Window") {}

window::window(int width, int height)
    : window(width, height, "Main Window") {}

window::window(int dim, const std::string &title)
    : window(dim, dim, title) {}

window::window(short mode, const std::string &title)
{
    if (!glfwInit())
        throw viewport_ex("Failed to create window. GLFW init failed");

    if (!(this->monitor = glfwGetPrimaryMonitor())) {
        glfwGetError(&this->err);
        glfwTerminate();

        throw viewport_ex("Failed to find monitor" + std::string(this->err));
    }

    const GLFWvidmode *m = glfwGetVideoMode(monitor);

    if (mode == WINDOW_MODE_FULLSCREEN) {
        this->fullscreen = true;
        this->maximize = false;
    }
    else if (mode == WINDOW_MODE_MAXIMIZED) {
        this->fullscreen = false;
        this->maximize = true;
    } else
        throw viewport_ex("Unknown window mode");

    this->width = m->width;
    this->height = m->height;
    this->title = title;

    this->create();
}

window::window(int width, int height, const std::string &title)
{
    if (!glfwInit())
        throw viewport_ex("Failed to create window. GLFW init failed");

    this->width = width;
    this->height = height;
    this->title = title;
    this->monitor = nullptr;
    this->fullscreen = false;
    this->maximize = false;

    this->create();
}

window::~window()
{
    glfwDestroyWindow(this->g_window);
    glfwTerminate();

    delete this->ctx;
    delete this->fb;
}

static void _close_cb()
{
    glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
}

void window::clear()
{
    glClearColor(this->clear_clr.r(), this->clear_clr.g(), this->clear_clr.b(), this->clear_clr.a());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::force_fullscreen()
{
    this->fullscreen = true;
    this->maximize = false;

    if (!this->monitor && !(this->monitor = glfwGetPrimaryMonitor())) {
        glfwGetError(&this->err);

        throw viewport_ex("Couldn't make window fullscreen: " +
                          std::string(this->err));
    }

    const GLFWvidmode *m = glfwGetVideoMode(this->monitor);
    if (!m) {
        glfwGetError(&this->err);

        throw viewport_ex("Couldn't make window fullscreen: " +
                          std::string(this->err));
    }

    glfwSetWindowMonitor(this->g_window,
                         this->monitor,
                         0,
                         0,
                         m->width,
                         m->height,
                         m->refreshRate);

    this->resize_framebuffer();
}

void window::resize(int width, int height)
{
    this->width = width;
    this->height = height;
    this->fullscreen = false;
    this->maximize = false;

    glfwSetWindowSize(this->g_window, this->width, this->height);

    this->resize_framebuffer();
}

void window::set_close_cb(close_cb cb)
{
    this->c_cb = cb;
}

void window::set_title(const std::string& title)
{
    this->title = title;

    glfwSetWindowTitle(this->g_window, this->title.c_str());
}

void window::update()
{
    if (!glfwWindowShouldClose(this->g_window)) {
        this->ctx->swap_buffers();
        glfwPollEvents();
    }
}

void *window::get_native_window()
{
    return static_cast<void *>(this->g_window);
}

GLFWwindow *window::get_g_window()
{
    return this->g_window;
}

GLFWmonitor *window::get_monitor()
{
    return this->monitor;
}

void window::resize_framebuffer()
{
    int w, h;

    glfwGetFramebufferSize(this->g_window, &w, &h);
    this->fb->resize(w, h);
}

void window::resize_framebuffer(GLFWwindow *w, int width, int height)
{
    this->fb->resize(width, height);
}

void window::window_set_close(GLFWwindow *w)
{
    (*this->c_cb)();
}


void window::create()
{
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    if (!(this->g_window = glfwCreateWindow(this->width,
                                            this->height,
                                            this->title.c_str(),
                                            this->fullscreen ? this->monitor : nullptr,
                                            nullptr))) {

        glfwGetError(&this->err);
        glfwTerminate();

        throw viewport_ex("Failed to create window: " + std::string(this->err));
    }

    this->ctx = graphics_context::create(this->g_window);
    this->ctx->init();

    glfwSwapInterval(0);

    this->fb = framebuffer::create();
    this->fb->set_clear_color(color(.66, .23, .54, 1));
    this->resize_framebuffer();

    this->clear_clr = color(0.8, 0.5, 0.2, 1.0);

    this->c_cb = _close_cb;
}


}
