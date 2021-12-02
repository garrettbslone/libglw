//
// Created by Garrett on 12/1/2021.
//

#ifndef GLW_DRAWABLE_HPP
#define GLW_DRAWABLE_HPP

#include "../ext/glm/glm/glm.hpp"
#include "../graphics/shader.hpp"

namespace glw {

class drawable {
public:
    virtual ~drawable() = 0;

    virtual void draw() = 0;

protected:
    glm::mat4 model;
    shader *shader_;
};

}

#endif //GLW_DRAWABLE_HPP
