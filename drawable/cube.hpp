//
// Created by Garrett on 12/2/21.
//

#ifndef GLW_CUBE_HPP
#define GLW_CUBE_HPP

#include "drawable.hpp"

namespace glw {

class cube : public drawable {
public:
    virtual ~cube();
    cube();

    void draw() override;
};

}

#endif //GLW_CUBE_HPP
