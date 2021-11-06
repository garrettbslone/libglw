//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_GL_VERTEX_ARRAY_HPP
#define GLW_GL_VERTEX_ARRAY_HPP

#include "../vertex_array.hpp"

namespace glw {

/*
 * An OpenGL defined vertex_array.
 */
class gl_vertex_array : public vertex_array {
public:
    gl_vertex_array();
    virtual ~gl_vertex_array();

    void bind() const override;
    void unbind() const override;

    void add_vertex_buffer(vertex_buffer *buff) override;
    void set_index_buffer(index_buffer *buff) override;

    inline const std::vector<vertex_buffer *> &get_vertex_buffers() const override { return this->v_buffs; }
    inline const index_buffer *get_index_buffer() const override { return this->i_buff; }

private:
    /*
     * gl_id: the index id of the vertex_array in the OpenGL state machine.
     * v_buff_index: the index of the next attribute to be bound.
     */
    uint32_t gl_id, v_buff_index = 0;
    std::vector<vertex_buffer *> v_buffs;
    index_buffer *i_buff;
};

}

#endif //GLW_GL_VERTEX_ARRAY_HPP
