//
// Created by Garrett on 9/26/21.
//

#ifndef GLW_MATH_HPP
#define GLW_MATH_HPP

#include <cmath>

namespace glw {

#define PI 3.14159265358979323846
#define DEG_TO_RADS(deg) ((deg) * PI / 180)
#define RAD_TO_DEGS(rad) ((rad) * 180 / PI)
#define TRIG_ROUND_ERR_DBL 0.0000000001
#define TRIG_ROUND_ERR_FLT 0.0000001

float trig_round_err(float v);

double trig_round_err(double v);

}

#endif //GLW_MATH_HPP
