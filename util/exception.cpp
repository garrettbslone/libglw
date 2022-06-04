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

io_ex::io_ex(const std::string &msg)
{
    this->msg = msg;
}

io_ex::~io_ex()
    = default;

component_ex::component_ex(const std::string &msg)
{
    this->msg = msg;
}

component_ex::~component_ex()
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

vulkan_load_ex::vulkan_load_ex(const std::string &msg)
{
    this->msg = msg;
}

vulkan_load_ex::~vulkan_load_ex()
    = default;

vulkan_debug_layers_ex::vulkan_debug_layers_ex(const std::string &msg)
{
    this->msg = msg;
}

vulkan_debug_layers_ex::~vulkan_debug_layers_ex()
    = default;

vulkan_device_ex::vulkan_device_ex(const std::string &msg)
{
    this->msg = msg;
}

vulkan_device_ex::~vulkan_device_ex()
    = default;

vulkan_image_ex::vulkan_image_ex(const std::string &msg)
{
    this->msg = msg;
}

vulkan_image_ex::~vulkan_image_ex()
    = default;

vulkan_buffer_ex::vulkan_buffer_ex(const std::string &msg)
{
    this->msg = msg;
}

vulkan_buffer_ex::~vulkan_buffer_ex()
    = default;

};
