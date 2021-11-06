//
// Created by Garrett on 10/6/21.
//

#include "math.hpp"

namespace glw {

float trig_round_err(float v)
{
    return abs(v) <= TRIG_ROUND_ERR_FLT ? (float) 0.0 : v;
}

double trig_round_err(double v)
{
    return abs(v) <= TRIG_ROUND_ERR_DBL ? 0.0 : v;
}

}