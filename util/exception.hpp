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

}

#endif //LIBGLW_EXCEPTION_HPP
