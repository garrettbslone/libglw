//
// Created by Garrett on 9/19/21.
//

#ifndef LIBGLW_EXCEPTION_HPP
#define LIBGLW_EXCEPTION_HPP

#include <exception>
#include <string>

namespace glw {

class out_of_bounds_ex : public std::exception {
public:
    std::string msg{};

    explicit out_of_bounds_ex(std::string msg);
    ~out_of_bounds_ex() override;
};

}

#endif //LIBGLW_EXCEPTION_HPP
