//
// Created by Garrett on 10/29/2021.
//

#ifndef GLW_TEXTURE_HPP
#define GLW_TEXTURE_HPP

#include <string>

namespace glw {

/*
 * An object representing an image.
 */
class texture {
public:
    virtual ~texture() = default;

    inline uint32_t get_width() const { return this->width; }
    inline uint32_t get_height() const { return this->height; }
    inline uint32_t get_gl_id() const { return this->gl_id; }
    inline bool is_loaded() const { return this->loaded; }

    virtual void set_data(void *data, uint32_t size) = 0;

    /*
     * Activates this texture in the gpu's texture slot 'slot'.
     */
    virtual void bind(uint32_t slot = 0) const = 0;
    /*
     * Deactivates this texture in the gpu's texture slot 'slot'.
     */
    virtual void unbind(uint32_t slot = 0) const = 0;

    virtual bool operator==(const texture &other) const = 0;

    /*
     * Create a graphics api specific texture.
     */
    static texture *create(const std::string &path);

protected:
    /*
     * gl_id: the index id of the vertex_array in the OpenGL state machine.
     */
    uint32_t width, height, gl_id;
    /*
     * The path to the image in the filesystem.
     */
    std::string path;
    bool loaded;
};

}

#endif //GLW_TEXTURE_HPP
