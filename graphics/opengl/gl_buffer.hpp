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
    gl_vertex_buffer(uint32_t size);
    gl_vertex_buffer(float *vertices, uint32_t size, uint32_t vert_size);
    virtual ~gl_vertex_buffer();

    void bind() const override;
    void unbind() const override;

    void set_data(const void *data, uint32_t size) override;
    inline std::vector<float> &get_data() override { return this->vertices; }
    inline unsigned int get_data_size() const override { return this->vertices.size(); }

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
    gl_index_buffer(uint32_t *indices, uint32_t count);
    virtual ~gl_index_buffer();

    void bind() const override;
    void unbind() const override;

    inline uint32_t get_count() const override { return this->count; }

private:
    /*
     * gl_id: the index id of the vertex_array in the OpenGL state machine.
     * count: the number of entries in the index buffer.
     */
    uint32_t gl_id, count;
};

}

#endif //GLW_GL_BUFFER_HPP
