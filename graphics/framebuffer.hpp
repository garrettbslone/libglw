//
// Created by Garrett on 10/12/21.
//

#ifndef GLW_FRAMEBUFFER_HPP
#define GLW_FRAMEBUFFER_HPP

#include <cstdint>

#include "color.hpp"
#include "graphics_api.hpp"
#include "swap_chain.hpp"

namespace glw {

typedef void (*framebuffer_size_cb)(int, int);

class framebuffer {
public:
    virtual ~framebuffer() = default;

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void destroy() = 0;
    virtual void reset() = 0;

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

    inline unsigned int get_width() const { return width; }
    inline unsigned int get_height() const { return height; }

    inline void set_resize_cb(framebuffer_size_cb cb) { this->fb_cb = cb; }
    inline void set_clear_color(const color& clr) { this->clear_clr = clr; }

    static framebuffer *create(swap_chain *chain, graphics_api api);

protected:
    color clear_clr;
    framebuffer_size_cb fb_cb;
    unsigned int width, height;
};

}

#endif //GLW_FRAMEBUFFER_HPP
