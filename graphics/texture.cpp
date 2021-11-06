//
// Created by Garrett on 10/29/2021.
//

#include "texture.hpp"
#include "opengl/gl_texture.hpp"

namespace glw {

texture *texture::create(const std::string &path)
{
    return new gl_texture(path);
}

}
