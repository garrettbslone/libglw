//
// Created by Garrett on 3/18/22.
//

#ifndef GLW_DESCRIPTOR_HPP
#define GLW_DESCRIPTOR_HPP

#include "../../ext/glm/glm/glm.hpp"

namespace glw {

struct global_ubo {
  glm::mat4 projection_view{1.f};
  glm::vec4 ambient_light_color{1.f, 1.f, 1.f, 0.02f}; // w is intensity
  glm::vec4 light_position{-1.f}; // ignore w -- for alignment purposes
  glm::vec4 light_color{1.f}; // w is intensity
};

/*
 * This can be ignored when using opengl as the api.
 */
class descriptor {

};

}

#endif //GLW_DESCRIPTOR_HPP
