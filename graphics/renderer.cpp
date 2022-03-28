//
// Created by Garrett on 3/18/22.
//

#include "renderer.hpp"

#include "opengl/gl_renderer.hpp"
#include "vulkan/vk_renderer.hpp"

namespace glw {

renderer *renderer::create(window *w, device *d, graphics_api api)
{
    renderer *r = nullptr;

    switch (api) {
        case API_VULKAN: {
            r = new vk_renderer(w, d);
            break;
        }
        case API_OPEN_GL:
        case API_NONE:
        default: {
            r = new gl_renderer(w, d);
            break;
        }
    }

    return r;
}

renderer::renderer(window *w, device *d)
    : window_(w), device_(d)
{

}

}