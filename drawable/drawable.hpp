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

// definition for topology data types
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
// public declarations of class elements
public:
    // drawable object destructor
    virtual ~drawable() = default;

    /*
     * default draw method meant to be overridden by specific
     * drawable object functions
     */
    virtual void draw() = 0;

    /*
     * getter and setter for the shaders needed for drawable
     * objects
     */
    inline shader *get_shader() const { return this->shader_; }
    inline void set_shader(shader *s) { this->shader_ = s; }

    /*
     * getter and setter for materials used for drawable
     * objects
     */
    inline void set_model_mat(const glm::mat4 &m) { this->model_ = m; }
    inline glm::mat4 get_model_mat() const { return this->model_; }

    /*
     * function to translate objects in 3D space
     * i.e. moving them about without rotation
     * or scaling
     */
    inline void translate(const glm::vec3 &v)
    {
        this->model_ = glm::translate(this->model_, v);
    }

    /*
     * function to rotate objects in 3D space
     * i.e. changing their orientation without translating
     * or scaling
     */
    inline void rotate(float deg, const glm::vec3 &v)
    {
        this->model_ = glm::rotate(this->model_, glm::radians(deg), v);
    }

    /*
     * function to change the size of an object
     * i.e. make an object bigger or smaller without
     * translating or scaling
     */
    inline void scale(const glm::vec3 &v)
    {
        this->model_ = glm::scale(this->model_, v);
    }

    /*
     * getter for position of a drawable object. returns
     * specific coordinates from matrix that represents location
     * in 3D space
     */
    inline glm::vec3 get_position()
    {
        return {
            this->model_[3][0],
            this->model_[3][1],
            this->model_[3][2]
        };
    }

// declaration of protected class elements
protected:
    // object of enum class defined above
    topology topology_;
    // matrix object representing the drawable
    glm::mat4 model_;
    // shader object used in fragment and vertex assigment
    shader *shader_;

    // index buffer object
    index_buffer *ib_;
    //vertex array object
    vertex_array *va_;
    //vertex buffer objects
    vertex_buffer *positions_, *uvs_, *normals_;
};

}

#endif //GLW_DRAWABLE_HPP
