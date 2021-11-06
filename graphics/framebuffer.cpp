//
// Created by Garrett on 10/12/21.
//

#include "framebuffer.hpp"
#include "opengl/gl_framebuffer.hpp"

namespace glw {

framebuffer *framebuffer::create()
{
    return new gl_framebuffer();
}

}
