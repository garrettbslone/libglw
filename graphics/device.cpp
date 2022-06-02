//
// Created by Garrett on 3/18/22.
//

#include "device.hpp"
#ifdef HAVE_VULKAN
#include "vulkan/vk_device.hpp"
#endif

namespace glw {

device *device::create(window *w, graphics_api api)
{
    switch (api) {
#ifdef HAVE_VULKAN
    case API_VULKAN:
        return new vk_device(w);
#endif
    case API_OPEN_GL:
    default:
        return nullptr;
    }
}

device::device(window *w)
    : window_(w)
{

}

}
