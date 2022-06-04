//
// Created by Garrett on 12/1/2021.
//

#ifndef GLW_DRAWABLE_HPP
#define GLW_DRAWABLE_HPP

#include "../ext/glm/glm/glm.hpp"
#include "../ext/glm/glm/ext.hpp"

#include "../graphics/buffer.hpp"
#include "../graphics/graphics_api.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/vertex_array.hpp"

namespace glw {

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

    inline glm::vec3 get_position()
    {
        return {
            this->model_[3][0],
            this->model_[3][1],
            this->model_[3][2]
        };
    }

protected:
    topology topology_;
    glm::mat4 model_;
    shader *shader_;

    index_buffer *ib_;
    vertex_array *va_;
    vertex_buffer *vertices_;
};

}

#endif //GLW_DRAWABLE_HPP
