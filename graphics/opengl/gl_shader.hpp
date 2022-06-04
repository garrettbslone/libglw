//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_GL_SHADER_HPP
#define GLW_GL_SHADER_HPP

#include <string>

#include "../shader.hpp"

namespace glw {

class gl_shader : public shader{
public:
    gl_shader(const std::string &name, const std::string &vertex_src, const std::string &fragment_src);
    virtual ~gl_shader();

    void bind() const override;
    void unbind() const override;

    void set_int(const std::string &name, int val) override;
    void set_int_arr(const std::string &name, int *vals, uint32_t cnt) override;
    void set_float(const std::string &name, float val) override;
    void set_float_arr(const std::string &name, float *vals, uint32_t cnt) override;
    void set_float2(const std::string &name, const glm::vec2 &v) override;
    void set_float3(const std::string &name, const glm::vec3 &v) override;
    void set_float4(const std::string &name, const glm::vec4 &v) override;
    void set_mat4(const std::string &name, const glm::mat4 &vm) override;

    /*
     * Pass some values to the vertex shader of this shader program as uniforms.
     */
    void set_uniform_int(const std::string &name, int val);
    void set_uniform_int_arr(const std::string &name, int *vals, uint32_t cnt);
    void set_uniform_float(const std::string &name, float val);
    void set_uniform_float_arr(const std::string &name, float *vals, uint32_t cnt);
    void set_uniform_float2(const std::string &name, const glm::vec2 &v);
    void set_uniform_float3(const std::string &name, const glm::vec3 &v);
    void set_uniform_float4(const std::string &name, const glm::vec4 &v);
    void set_uniform_mat3(const std::string &name, const glm::mat3 &m);
    void set_uniform_mat4(const std::string &name, const glm::mat4 &m);

private:
    /*
     * Reads two glsl shaders and compiles them into a shader program.
     * Throws a gl_load_ex exception on failure.
     */
    void read_glsl_files(const std::string &vertex_src_path, const std::string &fragment_src_path);

    /*
     * Compiles a shader of 'type' with 'src' and returns the gl id for said shader.
     */
    uint32_t compile(uint32_t type, const std::string &src);
};

}

#endif //GLW_GL_SHADER_HPP
