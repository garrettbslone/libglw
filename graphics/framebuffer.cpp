//
// Created by Garrett on 10/12/21.
//

#include "framebuffer.hpp"
#include "opengl/gl_framebuffer.hpp"

namespace glw {

framebuffer *framebuffer::create()
{
    /*
     * The target parameter for this object can take one of 3 values: GL_FRAMEBUFFER, GL_READ_FRAMEBUFFER, or GL_DRAW_FRAMEBUFFER.
     * In this case it is gl_framebuffer. This target simply sets both the read and write to the same FBO.
     */

    //Creates new framebuffer object
    return new gl_framebuffer();
}

}
