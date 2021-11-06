//
// Created by Garrett on 10/30/2021.
//

#include "shader.hpp"
#include "opengl/gl_shader.hpp"

namespace glw {

shader *shader::create(const std::string &name, const std::string &vertex_src, const std::string &fragment_src)
{
    return new gl_shader(name, vertex_src, fragment_src);
}

}
