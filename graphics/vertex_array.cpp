//
// Created by Garrett on 10/27/2021.
//

#include "vertex_array.hpp"
#include "opengl/gl_vertex_array.hpp"

namespace glw {

vertex_array *vertex_array::create()
{
    return new gl_vertex_array();
}

}
