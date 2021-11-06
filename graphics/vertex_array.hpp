//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_VERTEX_ARRAY_HPP
#define GLW_VERTEX_ARRAY_HPP

#include <vector>

#include "buffer.hpp"

namespace glw {

/*
 * An object that allows for adding attachments and offsets to vertex buffers.
 */
class vertex_array {
public:
    virtual ~vertex_array() = default;

    /*
     * Activate this vertexarray allowing its attributes to be used.
     */
    virtual void bind() const = 0;
    /*
     * Deactivate this vertex_array.
     */
    virtual void unbind() const = 0;

    virtual void add_vertex_buffer(vertex_buffer *buff) = 0;
    virtual void set_index_buffer(index_buffer *buff) = 0;

    virtual const std::vector<vertex_buffer *> &get_vertex_buffers() const = 0;
    virtual const index_buffer *get_index_buffer() const = 0;

    /*
     * Create a graphics api specific vertex array.
     */
    static vertex_array *create();
};

}

#endif //GLW_VERTEX_ARRAY_HPP
