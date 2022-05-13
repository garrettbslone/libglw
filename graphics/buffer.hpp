//
// Created by Garrett on 10/27/2021.
//

#ifndef GLW_BUFFER_HPP
#define GLW_BUFFER_HPP

#include "../drawable/vertex.hpp"

#include <cstdint>
#include <vector>

namespace glw {

enum class buffer_type {
    INDEX,
    GENERIC,
    VERTEX
};

#define INDEX_BUFFER buffer_type::INDEX
#define GENERIC_BUFFER buffer_type::GENERIC
#define VERTEX_BUFFER buffer_type::VERTEX

struct vertex_data {
  bool is_float{};
  bool interleaved{};

  union {
    vertex v;
    float f{};
  }
};

/*
 * A generic abstract buffer of data to be stored on the GPU.
 */
class buffer {
public:
    virtual ~buffer() = default;

    /*
     * Activates the current buffer.
     */
    virtual void bind(class command_buffer *cmds) const = 0;
    /*
     * Deactivates the current buffer.
     */
    virtual void unbind() const = 0;

    virtual void write_data(void *data, uint64_t size, uint64_t offset) = 0;

    static buffer *create(
                class device *d,
                uint64_t size,
                uint32_t count,
                uint32_t usage,
                uint32_t memory_usage,
                buffer_type type = GENERIC_BUFFER
            );
};

/*
 * A buffer to hold vertex data (positions, normals, texcords, etc.) in VRAM.
 */
class vertex_buffer : public buffer {
public:
    virtual ~vertex_buffer() = default;

    inline std::vector<vertex_data> &get_data() { return this->vertices_; }
    inline unsigned int get_data_size() const { return this->vertices_.size(); }

    /*
     * Create a graphics api specific vertex_buffer.
     */
    static vertex_buffer *create(
            const std::vector<vertex_data> &vertices,
            unsigned int vertex_size = 3
    );

protected:
    std::vector<vertex_data> vertices_;
};

class index_buffer : public buffer {
public:
    virtual ~index_buffer() = default;

    inline uint32_t get_count() const { return this->count_; }

    /*
     * Create a graphics api specific index_buffer.
     */
    static index_buffer *create(const std::vector<uint32_t> &indices);

protected:
    uint32_t count_;
};

class command_buffer {
public:
    command_buffer() = default;
    virtual ~command_buffer() = default;

    virtual void *raw() = 0;

    static command_buffer *create();
};

}

#endif //GLW_BUFFER_HPP
