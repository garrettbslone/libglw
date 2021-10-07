//
// Created by Garrett on 9/19/21.
//

#include "exception.hpp"

namespace glw {

out_of_bounds_ex::out_of_bounds_ex(std::string msg)
{
    this->msg = msg;
}

out_of_bounds_ex::~out_of_bounds_ex()
        = default;

div_by_zero_ex::div_by_zero_ex(std::string msg)
{
    this->msg = msg;
}

div_by_zero_ex::~div_by_zero_ex()
        = default;

};
