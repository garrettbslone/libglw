//
// Created by Garrett on 10/27/2021.
//
#include "gl_framebuffer.hpp"
#include "../../ext/glad/gl.h"

namespace glw {

void _framebuffer_resize_cb(int width, int height)
{
    glViewport(0, 0, width, height);
}

gl_framebuffer::gl_framebuffer()
{
    glGenFramebuffers(1, &this->fb_id);
    this->fb_cb = &_framebuffer_resize_cb;
}

gl_framebuffer::~gl_framebuffer()
{
    glDeleteFramebuffers(1, &this->fb_id);
}

void gl_framebuffer::init()
{
    this->reset();
    this->fb_cb = &_framebuffer_resize_cb;
}

void gl_framebuffer::clear()
{
    glClearColor(this->clear_clr.r(), this->clear_clr.g(), this->clear_clr.b(), this->clear_clr.a());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gl_framebuffer::destroy()
{
    if (this->fb_id) {
        glDeleteFramebuffers(1, &this->fb_id);
        this->fb_id = 0;
    }
}

void gl_framebuffer::reset()
{
    this->destroy();
    this->clear();

    glGenFramebuffers(1, &this->fb_id);
}

void gl_framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->fb_id);
    glViewport(0, 0, this->width, this->height);
}

void gl_framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gl_framebuffer::resize(uint32_t width, uint32_t height)
{
    (*this->fb_cb)(width, height);
}

}