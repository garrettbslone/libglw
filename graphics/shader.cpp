//
// Created by Garrett on 10/30/2021.
//

#include <fstream>

#include "shader.hpp"
#include "opengl/gl_shader.hpp"
#ifdef HAVE_VULKAN
#include "vulkan/vk_shader.hpp"
#endif

#include "../util/exception.hpp"

namespace glw {

std::string shader::read_file(const std::string &f_path)
{
    std::ifstream fin(f_path, std::ifstream::in | std::ifstream::binary);

    if (!fin)
        throw io_ex("Couldn't read shader source from: " + f_path);

    fin.seekg(0, std::ifstream::end);
    std::string res((size_t) fin.tellg(), '\0');
    fin.seekg(0, std::ifstream::beg);
    fin.read(&res[0], res.size());
    fin.close();

    return res;
}

shader *shader::create(
        const std::string &name,
        const std::string &vertex_src,
        const std::string &fragment_src
)
{
    switch (api::active) {
#ifdef HAVE_VULKAN
    case API_VULKAN:
        return new vk_shader(name, vertex_src, fragment_src);
#endif
    case API_OPEN_GL:
    default:
        return new gl_shader(name, vertex_src, fragment_src);
    }
}

}
