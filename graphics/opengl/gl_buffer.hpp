//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_GL_BUFFER_HPP
#define GLW_GL_BUFFER_HPP

#include "../buffer.hpp"

namespace glw {

/*
 * An OpenGL defined vertex_buffer.
 */
class gl_vertex_buffer : public vertex_buffer {
public:
    gl_vertex_buffer(const std::vector<vertex> &vertices);
    virtual ~gl_vertex_buffer();

    void bind(command_buffer *cmds) const override;
    void unbind() const override;

    void write_data(void *vertices, uint64_t size, uint64_t offset) override;

private:
    /*
     * gl_id: the index id of the vertex_array in the OpenGL state machine.
     */
    uint32_t gl_id;
};

/*
 * An OpenGL defined index_buffer.
 */
class gl_index_buffer : public index_buffer {
public:
    gl_index_buffer(const std::vector<uint32_t> &indices);
    virtual ~gl_index_buffer();

    void bind(command_buffer *cmds) const override;
    void unbind() const override;

    void write_data(void *indices, uint64_t size, uint64_t offset) override;

private:
    /*
     * gl_id: the index id of the vertex_array in the OpenGL state machine.
     */
    uint32_t gl_id;
};

}

#endif //GLW_GL_BUFFER_HPP
