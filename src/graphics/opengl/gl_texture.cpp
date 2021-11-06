//
// Created by Garrett on 10/29/2021.
//

#include "gl_texture.hpp"
#include "../../util/exception.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace glw {

gl_texture::gl_texture(uint32_t width, uint32_t height)
{
    this->width = width;
    this->height = height;
    this->path = "";
    this->loaded = false;

    this->internal_format = GL_RGBA8;
    this->data_format = GL_RGBA;

    glGenTextures(1, &this->gl_id);

    glTexParameteri(this->gl_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(this->gl_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(this->gl_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(this->gl_id, GL_TEXTURE_WRAP_R, GL_REPEAT);
}

gl_texture::gl_texture(const std::string &path)
{
    this->width = -1;
    this->height = -1;
    this->path = path;

    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc *data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (!data)
        throw gl_load_ex("failed to load texture at: " + path + "\n" + stbi_failure_reason());

    this->loaded = true;
    this->width = w;
    this->height = h;

    if (channels == 4) {
        this->internal_format = GL_RGBA8;
        this->data_format = GL_RGBA;
    } else {
        this->internal_format = GL_RGB8;
        this->data_format = GL_RGB;
    }

    glGenTextures(1, &this->gl_id);

    glTexParameteri(this->gl_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(this->gl_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(this->gl_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(this->gl_id, GL_TEXTURE_WRAP_R, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 this->data_format,
                 this->width,
                 this->height,
                 0,
                 this->data_format,
                 GL_UNSIGNED_BYTE,
                 data);

    stbi_image_free(data);
}

gl_texture::~gl_texture()
{
    glDeleteTextures(1, &this->gl_id);
}

void gl_texture::set_data(void *data, uint32_t size)
{
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 this->internal_format,
                 this->width,
                 this->height,
                 0,
                 this->internal_format,
                 GL_UNSIGNED_BYTE,
                 data);
}

void gl_texture::bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->gl_id);
}

void gl_texture::unbind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool gl_texture::operator==(const texture &other) const
{
    return this->gl_id == ((gl_texture &) other).gl_id;
}

}
