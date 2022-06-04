//
// Created by Garrett on 9/19/21.
//

#ifndef LIBGLW_EXCEPTION_HPP
#define LIBGLW_EXCEPTION_HPP

#include <exception>
#include <string>

namespace glw {

class glw_runtime_ex : public std::exception {
public:
    std::string msg{};
};

class out_of_bounds_ex : public glw_runtime_ex {
public:
    explicit out_of_bounds_ex(const std::string& msg);
    ~out_of_bounds_ex() override;
};

class div_by_zero_ex : public glw_runtime_ex {
public:
    explicit div_by_zero_ex(const std::string& msg);
    ~div_by_zero_ex() override;
};

class io_ex : public glw_runtime_ex {
public:
    explicit io_ex(const std::string &msg);
    ~io_ex() override;
};

class component_ex : public glw_runtime_ex {
public:
    explicit component_ex(const std::string &msg);
    ~component_ex() override;
};

class viewport_ex : public glw_runtime_ex {
public:
    explicit viewport_ex(const std::string& msg);
    ~viewport_ex() override;
};

class gl_load_ex : public glw_runtime_ex {
public:
    explicit gl_load_ex(const std::string &msg);
    ~gl_load_ex() override;
};

class app_ex : public glw_runtime_ex {
public:
    explicit app_ex(const std::string &msg);
    ~app_ex() override;
};

class vulkan_load_ex : public glw_runtime_ex {
public:
    explicit vulkan_load_ex(const std::string &msg);
    ~vulkan_load_ex() override;
};

class vulkan_debug_layers_ex : public glw_runtime_ex {
public:
    explicit vulkan_debug_layers_ex(const std::string &msg);
    ~vulkan_debug_layers_ex() override;
};

class vulkan_device_ex : public glw_runtime_ex {
public:
    explicit vulkan_device_ex(const std::string &msg);
    ~vulkan_device_ex() override;
};

class vulkan_image_ex : public glw_runtime_ex {
public:
    explicit vulkan_image_ex(const std::string &msg);
    ~vulkan_image_ex() override;
};

class vulkan_buffer_ex : public glw_runtime_ex {
public:
    explicit vulkan_buffer_ex(const std::string &msg);
    ~vulkan_buffer_ex() override;
};

}

#endif //LIBGLW_EXCEPTION_HPP
