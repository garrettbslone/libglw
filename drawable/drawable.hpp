//
// Created by Garrett on 12/1/2021.
//

#ifndef GLW_DRAWABLE_HPP
#define GLW_DRAWABLE_HPP

#include "../ext/glm/glm/glm.hpp"
#include "../ext/glm/glm/ext.hpp"

#include "../graphics/buffer.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/vertex_array.hpp"

namespace glw {

enum class topology {
    POINTS,
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
};

class drawable {
public:
    virtual ~drawable() = default;

    virtual void draw() = 0;

    inline shader *get_shader() const { return this->shader_; }
    inline void set_shader(shader *s) { this->shader_ = s; }

    inline void set_model_mat(const glm::mat4 &m) { this->model_ = m; }
    inline glm::mat4 get_model_mat() const { return this->model_; }

    inline void translate(const glm::vec3 &v)
    {
        this->model_ = glm::translate(this->model_, v);
    }

    inline void rotate(float deg, const glm::vec3 &v)
    {
        this->model_ = glm::rotate(this->model_, glm::radians(deg), v);
    }

    inline void scale(const glm::vec3 &v)
    {
        this->model_ = glm::scale(this->model_, v);
    }

protected:
    topology topology_;
    glm::mat4 model_;
    shader *shader_;

    index_buffer *ib_;
    vertex_array *va_;
    vertex_buffer *positions_, *uvs_, *normals_;
};

}

#endif //GLW_DRAWABLE_HPP
