//
// Created by Garrett on 10/12/21.
//

#ifndef GLW_FRAMEBUFFER_HPP
#define GLW_FRAMEBUFFER_HPP

#include <cstdint>

#include "color.hpp"
/**
 * This is a framebuffer class.
 * The purpose of a framebuffer is to store buffers for internal rendering.
 * A framebuffer is memory that stores color values for each pixel on display.
 */
namespace glw {

typedef void (*framebuffer_size_cb)(int, int);

class framebuffer {
// public declarations of class elements
public:
    //Destroys object and allocated resources.
    virtual ~framebuffer() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void destroy() = 0;
    virtual void reset() = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

    /*
     * Getters and setters for width and height
     * needed for framebuffer
     */
    inline unsigned int get_width() const { return width; }
    inline unsigned int get_height() const { return height; }

    /*
     * Setters for resizing window. Callback function
     */
    inline void set_resize_cb(framebuffer_size_cb cb) { this->fb_cb = cb; }

    /*
     * Setter for setting a color
     */
    inline void set_clear_color(const color& clr) { this->clear_clr = clr; }

    // Creates framebuffer
    static framebuffer *create();

// declaration of protected class elements
protected:
    // color object
    color clear_clr;
    //framebuffer size object
    framebuffer_size_cb fb_cb;
    //width and height
    unsigned int width, height;
};

}

#endif //GLW_FRAMEBUFFER_HPP
