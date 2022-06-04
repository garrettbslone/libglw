//
// Created by Garrett on 12/2/21.
//

#ifndef GLW_TOPOLOGY_HPP
#define GLW_TOPOLOGY_HPP

#include "../../ext/glad/gl.h"
#include "graphics_api.hpp"

namespace glw {

class gl_topology {
public:
    static inline GLenum get_gl_topology(topology t)
    {
        switch (t) {
            case topology::POINTS: return GL_POINTS;
            case topology::LINES: return GL_LINES;
            case topology::LINE_LOOP: return GL_LINE_LOOP;
            case topology::LINE_STRIP: return GL_LINE_STRIP;
            case topology::TRIANGLES: return GL_TRIANGLES;
            case topology::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
            case topology::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
            default: return GL_TRIANGLES;
        }
    }
};

}

#endif //GLW_TOPOLOGY_HPP
