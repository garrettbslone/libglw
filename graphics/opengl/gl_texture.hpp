//
// Created by Garrett on 10/29/2021.
//

#ifndef GLW_GL_TEXTURE_HPP
#define GLW_GL_TEXTURE_HPP

#include "../texture.hpp"
#include "../../ext/glad/gl.h"

namespace glw {

class gl_texture : public texture{
public:
    gl_texture(uint32_t width, uint32_t height);
    gl_texture(const std::string &path);
    virtual ~gl_texture();

    void set_data(void *data, uint32_t size) override;

    void bind(uint32_t slot) const override;
    void unbind(uint32_t slot = 0) const override;

    bool operator==(const texture &other) const override;

private:
    /*
     * internal_format: the number of color components in the texture
     *                  (usually RGBA8 or RGB8).
     * data_format: the format of the pixel data (usually RGBA or RGB).
     */
    GLenum internal_format, data_format;
};

}

#endif //GLW_GL_TEXTURE_HPP
