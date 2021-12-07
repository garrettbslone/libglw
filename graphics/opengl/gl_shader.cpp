//
// Created by Garrett on 10/30/2021.
//

#include <fstream>

#include "gl_shader.hpp"
#include "../../util/exception.hpp"
#include "../../ext/glad/gl.h"

namespace glw {

gl_shader::gl_shader(const std::string &name, const std::string &vertex_src, const std::string &fragment_src)
{
    this->name = name;
    this->read_glsl_files(vertex_src, fragment_src);
}

gl_shader::~gl_shader()
{
    glDeleteProgram(this->gl_id);
}

void gl_shader::bind() const
{
    glUseProgram(this->gl_id);
}

void gl_shader::unbind() const
{
    glUseProgram(0);
}

void gl_shader::set_int(const std::string &name, int val)
{
    this->set_uniform_int(name, val);
}

void gl_shader::set_int_arr(const std::string &name, int *vals, uint32_t cnt)
{
    this->set_uniform_int_arr(name, vals, cnt);
}

void gl_shader::set_float(const std::string &name, float val)
{
    this->set_uniform_float(name, val);
}

void gl_shader::set_float_arr(const std::string &name, float *vals, uint32_t cnt)
{
    this->set_uniform_float_arr(name, vals, cnt);
}

void gl_shader::set_float2(const std::string &name, const glm::vec2 &v)
{
    this->set_uniform_float2(name, v);
}

void gl_shader::set_float3(const std::string &name,const glm::vec3 &v)
{
    this->set_uniform_float3(name, v);
}

void gl_shader::set_float4(const std::string &name, const glm::vec4 &v)
{
    this->set_uniform_float4(name, v);
}

void gl_shader::set_mat4(const std::string &name, const glm::mat4 &m)
{
    this->set_uniform_mat4(name, m);
}

void gl_shader::set_uniform_int(const std::string &name, int val)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniform1i(loc, val);
}

void gl_shader::set_uniform_int_arr(const std::string &name, int *vals, uint32_t cnt)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniform1iv(loc, cnt, vals);
}

void gl_shader::set_uniform_float(const std::string &name, float val)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniform1f(loc, val);
}

void gl_shader::set_uniform_float_arr(const std::string &name, float *vals, uint32_t cnt)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniform1fv(loc, cnt, vals);
}

void gl_shader::set_uniform_float2(const std::string &name, const glm::vec2 &v)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniform2f(loc, v.x, v.y);
}

void gl_shader::set_uniform_float3(const std::string &name, const glm::vec3 &v)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniform3f(loc, v.x, v.y, v.z);
}

void gl_shader::set_uniform_float4(const std::string &name, const glm::vec4 &v)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void gl_shader::set_uniform_mat3(const std::string &name, const glm::mat3 &m)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void gl_shader::set_uniform_mat4(const std::string &name, const glm::mat4 &m)
{
    GLint loc = glGetUniformLocation(this->gl_id, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

std::string gl_shader::read_file(const std::string &f_path)
{
    std::ifstream fin(f_path, std::ifstream::in | std::ifstream::binary);

    if (!fin)
        throw gl_load_ex("Couldn't read shader source from: " + f_path);

    fin.seekg(0, std::ifstream::end);
    std::string res((size_t) fin.tellg(), '\0');
    fin.seekg(0, std::ifstream::beg);
    fin.read(&res[0], res.size());
    fin.close();

    return res;
}

void gl_shader::read_glsl_files(const std::string &vertex_src_path, const std::string &fragment_src_path)
{
    auto vertex_src = this->read_file(vertex_src_path);
    auto fragment_src = this->read_file(fragment_src_path);

    this->gl_id = glCreateProgram();

    GLuint vertex_shader = this->compile(GL_VERTEX_SHADER, vertex_src);
    glAttachShader(this->gl_id, vertex_shader);
    GLuint fragment_shader = this->compile(GL_FRAGMENT_SHADER, fragment_src);
    glAttachShader(this->gl_id, fragment_shader);

    glLinkProgram(this->gl_id);

    GLint linked = 0;
    glGetProgramiv(this->gl_id, GL_LINK_STATUS, (int *) &linked);

    if (!linked) {
        GLint max_len = 0;
        glGetProgramiv(this->gl_id, GL_INFO_LOG_LENGTH, &max_len);

        auto *log = new GLchar[max_len];
        glGetProgramInfoLog(this->gl_id, max_len, &max_len, log);

        glDeleteProgram(this->gl_id);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        throw gl_load_ex("Couldn't compile shader\n" + std::string(log));
    }

    glDetachShader(this->gl_id, vertex_shader);
    glDetachShader(this->gl_id, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

uint32_t gl_shader::compile(uint32_t type, const std::string &src)
{
    GLuint shader = glCreateShader(type);

    auto src_c_str = src.c_str();
    glShaderSource(shader, 1, &src_c_str, nullptr);
    glCompileShader(shader);

    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint max_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);

        auto *log = new GLchar[max_len];
        glGetShaderInfoLog(shader, max_len, &max_len, log);

        glDeleteShader(shader);

        throw gl_load_ex("Couldn't compile shader\n" + src + "\n" + std::string(log));
    }

    return shader;
}

}
