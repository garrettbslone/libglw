//
// Created by Garrett on 9/19/21.
//

#include "exception.hpp"

namespace glw {

out_of_bounds_ex::out_of_bounds_ex(const std::string& msg)
{
    this->msg = msg;
}

out_of_bounds_ex::~out_of_bounds_ex()
        = default;

div_by_zero_ex::div_by_zero_ex(const std::string& msg)
{
    this->msg = msg;
}

div_by_zero_ex::~div_by_zero_ex()
        = default;

viewport_ex::viewport_ex(const std::string& msg)
{
    this->msg = msg;
}

viewport_ex::~viewport_ex()
        = default;

gl_load_ex::gl_load_ex(const std::string &msg)
{
    this->msg = msg;
}

gl_load_ex::~gl_load_ex()
        = default;

app_ex::app_ex(const std::string &msg)
{
    this->msg = msg;
}

app_ex::~app_ex()
        = default;

};
