//
// Created by Garrett on 12/6/2021.
//

#ifndef GLW_QUAD_HPP
#define GLW_QUAD_HPP

#include "drawable.hpp"

namespace glw {

class quad : public drawable {
public:
    virtual ~quad();
    quad();

    void draw() override;
};

}

#endif //GLW_QUAD_HPP
