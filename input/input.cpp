//
// Created by Garrett on 11/25/21.
//

#include "input.hpp"

namespace glw {

input::input() = default;
input::~input() = default;

float input::get_mouse_x()
{
    return 0;
}

float input::get_mouse_y()
{
    return 0;
}

std::pair<float, float> input::get_mouse_pos()
{
    return std::pair<float, float>();
}

}
