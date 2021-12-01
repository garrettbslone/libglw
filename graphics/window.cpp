//
// Created by Garrett on 9/28/2021.
//

#include <utility>

#define GLAD_GL_IMPLEMENTATION
#include "../ext/glad/gl.h"

#include "../util/exception.hpp"
#include "window.hpp"


namespace glw {

window::window(const window_spec &spec)
{
    if (!glfwInit())
        throw viewport_ex("Failed to create window. GLFW init failed");

    this->spec_ = spec;

    if (!(this->monitor_ = glfwGetPrimaryMonitor())) {
        glfwGetError(&this->err);
        glfwTerminate();

        throw viewport_ex("Failed to find monitor_" + std::string(this->err));
    }

    const GLFWvidmode *m = glfwGetVideoMode(monitor_);

    if (this->spec_.maximized_) {
        this->spec_.width_ = m->width;
        this->spec_.height_ = m->height;
    }

    this->create();
}

window::~window()
{
    glfwDestroyWindow(this->native_window_);
    glfwTerminate();

    delete this->graphics_ctx_;
    delete this->fb_;
}

static void _close_cb()
{
    glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
}

void window::clear()
{
    glClearColor(this->clear_clr_.r(), this->clear_clr_.g(), this->clear_clr_.b(), this->clear_clr_.a());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::force_fullscreen()
{
    this->spec_.fullscreen_ = true;
    this->spec_.maximized_ = false;

    if (!this->monitor_ && !(this->monitor_ = glfwGetPrimaryMonitor())) {
        glfwGetError(&this->err);

        throw viewport_ex("Couldn't make window fullscreen: " +
                          std::string(this->err));
    }

    const GLFWvidmode *m = glfwGetVideoMode(this->monitor_);
    if (!m) {
        glfwGetError(&this->err);

        throw viewport_ex("Couldn't make window fullscreen: " +
                          std::string(this->err));
    }

    glfwSetWindowMonitor(this->native_window_,
                         this->monitor_,
                         0,
                         0,
                         m->width,
                         m->height,
                         m->refreshRate);

    this->resize_framebuffer();
}

void window::resize(uint32_t width, uint32_t height)
{
    this->spec_.width_ = width;
    this->spec_.height_ = height;
    this->spec_.fullscreen_ = false;
    this->spec_.maximized_ = false;

    glfwSetWindowSize(this->native_window_, width, height);

    this->resize_framebuffer();
}

void window::set_close_cb(close_cb cb)
{
    this->data_.close_ = std::move(cb);
}

void window::set_title(const std::string& title)
{
    this->spec_.title_ = title;

    glfwSetWindowTitle(this->native_window_, title.c_str());
}

bool window::update()
{
    if (!glfwWindowShouldClose(this->native_window_)) {
        this->graphics_ctx_->swap_buffers();
        glfwPollEvents();
        return false;
    }

    return true;
}

void *window::get_native_window()
{
    return static_cast<void *>(this->native_window_);
}

GLFWwindow *window::get_g_window()
{
    return this->native_window_;
}

GLFWmonitor *window::get_monitor()
{
    return this->monitor_;
}

void window::resize_framebuffer()
{
    int w, h;

    glfwGetFramebufferSize(this->native_window_, &w, &h);
    this->fb_->resize(w, h);
}

void window::resize_framebuffer(GLFWwindow *w, uint32_t width, uint32_t height)
{
    this->fb_->resize(width, height);
}

void window::window_set_close(GLFWwindow *w)
{
    if (this->data_.close_)
        this->data_.close_();
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

    if (!(this->native_window_ = glfwCreateWindow(this->spec_.width_,
                                                  this->spec_.height_,
                                                  this->spec_.title_.c_str(),
                                                  this->spec_.fullscreen_ ? this->monitor_ : nullptr,
                                                  nullptr))) {

        glfwGetError(&this->err);
        glfwTerminate();

        throw viewport_ex("Failed to create window: " + std::string(this->err));
    }

    this->graphics_ctx_ = graphics_context::create(this->native_window_);
    this->graphics_ctx_->init();

    this->data_.close_ = _close_cb;
    glfwSetWindowUserPointer(this->native_window_, &this->data_);

    glfwSwapInterval(0);
    glfwSetWindowCloseCallback(this->native_window_, [](GLFWwindow *w) {
        auto &data = *((window_data *) glfwGetWindowUserPointer(w));

        if (data.close_)
            data.close_();
    });

    this->fb_ = framebuffer::create();
    this->fb_->set_clear_color(color(.66, .23, .54, 1));
    this->resize_framebuffer();

    this->clear_clr_ = color(0.8, 0.5, 0.2, 1.0);

    this->data_.close_ = _close_cb;
}


}
