//
// Created by Garrett on 3/18/22.
//

#include "renderer.hpp"

#include "opengl/gl_renderer.hpp"
#ifdef HAVE_VULKAN
#include "vulkan/vk_renderer.hpp"
#endif

namespace glw {

renderer *renderer::create(window *w)
{
    switch (api::active) {
#ifdef HAVE_VULKAN
    case API_VULKAN:
            return new vk_renderer(w);
#endif
        case API_OPEN_GL:
        default:
            return new gl_renderer(w);
    }
}

renderer::renderer(window *w)
    : window_(w)
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