//
// Created by Garrett on 3/18/22.
//

#include "renderer.hpp"

#include "opengl/gl_renderer.hpp"

namespace glw {

renderer *renderer::create(window *w, device *d, graphics_api api)
{
    renderer *r = nullptr;

    switch (api) {
        case API_VULKAN: {
            r = new gl_renderer(w, d);
        }
        case API_OPEN_GL:
        case API_NONE:
        default: {
            r = new gl_renderer(w, d);
        }
    }

    return r;
}

renderer::renderer(window *w, device *d)
    : window_(w), device_(d)
{

}

void renderer::create_command_buffers()
{

}

void renderer::free_command_buffers()
{

}

void renderer::recreate_swap_chain()
{

}

}