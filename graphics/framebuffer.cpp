//
// Created by Garrett on 10/12/21.
//

#include "framebuffer.hpp"
#include "opengl/gl_framebuffer.hpp"
#ifdef HAVE_VULKAN
#include "vulkan/vk_framebuffer.hpp"
#endif

namespace glw {

framebuffer *framebuffer::create(swap_chain *chain)
{
    switch (api::active) {
#ifdef HAVE_VULKAN
    case API_VULKAN:
        return new vk_framebuffer(chain);
#endif
    case API_OPEN_GL:
    case API_NONE:
    default:
        return new gl_framebuffer();
    }
}

}
