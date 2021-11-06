//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_BUFFER_HPP
#define GLW_BUFFER_HPP

#include <cstdint>
#include <vector>

namespace glw {

/*
 * A generic abstract buffer of data to be stored on the GPU.
 */
class buffer {
public:
    virtual ~buffer() = default;

    /*
     * Activates the current buffer.
     */
    virtual void bind() const = 0;
    /*
     * Deactivates the current buffer.
     */
    virtual void unbind() const = 0;
};

/*
 * A buffer to hold vertex data (positions, normals, texcords, etc.) in VRAM.
 */
class vertex_buffer : public buffer {
public:
    virtual ~vertex_buffer() = default;

    virtual void set_data(const void *data, uint32_t size) = 0;
    virtual std::vector<float> &get_data() = 0;
    virtual unsigned int get_data_size() const = 0;

    /*
     * Create a graphics api specific vertex_buffer.
     */
    static vertex_buffer *create(uint32_t size);
    static vertex_buffer *create(float *vertices, uint32_t size);

protected:
    std::vector<float> vertices;
};

class index_buffer : public buffer {
public:
    virtual ~index_buffer() = default;

    virtual uint32_t get_count() const = 0;

    /*
     * Create a graphics api specific index_buffer.
     */
    static index_buffer *create(uint32_t *indices, uint32_t count);
};

}

#endif //GLW_BUFFER_HPP
