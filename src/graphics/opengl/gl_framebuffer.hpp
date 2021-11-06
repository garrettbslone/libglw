//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_GL_FRAMEBUFFER_HPP
#define GLW_GL_FRAMEBUFFER_HPP

#include <cstdint>
#include "../framebuffer.hpp"

namespace glw {

class gl_framebuffer : public framebuffer {
public:
    gl_framebuffer();
    virtual ~gl_framebuffer();

    void init() override;
    void clear() override;
    void destroy() override;
    void reset() override;

    void bind() override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height) override;

private:
    uint32_t fb_id = 0;
};

}

#endif //GLW_GL_FRAMEBUFFER_HPP
