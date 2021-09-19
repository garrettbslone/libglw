//
// Created by Garrett on 9/19/21.
//

#include "exception.hpp"

namespace glw {

out_of_bounds_ex::out_of_bounds_ex(std::string msg)
        :msg(msg) { }

out_of_bounds_ex::~out_of_bounds_ex()
= default;

};
