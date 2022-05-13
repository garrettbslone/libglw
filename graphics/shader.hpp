//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_SHADER_HPP
#define GLW_SHADER_HPP

#include <string>
#include <glm/glm.hpp>

#include "device.hpp"
#include "graphics_api.hpp"

namespace glw {

/*
 * A class that represents a shader program containing both a vertex and a
 * fragment shader.
 */
class shader {
public:
    virtual ~shader() = default;

    /*
     * Enable this shader program to be used.
     */
    virtual void bind() const = 0;
    /*
     * Disable this shader program.
     */
    virtual void unbind() const = 0;

    /*
     * Pass some values to the vertex shader of this shader program.
     */
    virtual void set_int(const std::string &name, int val) = 0;
    virtual void set_int_arr(const std::string &name, int *vals, uint32_t cnt) = 0;
    virtual void set_float(const std::string &name, float val) = 0;
    virtual void set_float_arr(const std::string &name, float *vals, uint32_t cnt) = 0;
    virtual void set_float2(const std::string &name, const glm::vec2 &v) = 0;
    virtual void set_float3(const std::string &name, const glm::vec3 &v) = 0;
    virtual void set_float4(const std::string &name, const glm::vec4 &v) = 0;
    virtual void set_mat4(const std::string &name, const glm::mat4 &m) = 0;

    inline const std::string &get_name() const { return this->name; }

    /*
     * Read the contents of 'f_path' as a string.
     */
    static std::string read_file(const std::string &f_path);
    /*
    * Create a graphics api specific shader program.
    */
    static shader *create(const std::string &name,
            const std::string &vertex_src,
            const std::string &fragment_src,
            device *dev,
            graphics_api api);

protected:
    /*
     * gl_id: the index id of the vertex_array in the OpenGL state machine.
     * name: a name for the shader program.
     */
    uint32_t gl_id;
    std::string name;
};

}

#endif //GLW_SHADER_HPP
