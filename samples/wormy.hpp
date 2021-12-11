//
// Created by Garrett on 12/7/2021.
//

#ifndef WORMY_HPP
#define WORMY_HPP

#include <glw.hpp>

using namespace glw;

class wormy : public drawable {
public:
    virtual ~wormy();
    wormy(int vertex_count, int r = 1);

    void draw() override;

};

#endif //WORMY_HPP
