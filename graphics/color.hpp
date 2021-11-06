//
// Created by Garrett on 10/12/21.
//

#ifndef GLW_COLOR_HPP
#define GLW_COLOR_HPP

#include <string>

namespace glw {

/*
 * An object that represents an RGBA color using float values [0-1] for each
 * channel.
 */
class color {
public:
    constexpr static const double CHANNEL_MIN = 0.0, CHANNEL_MAX = 1.0;
    static const int R = 0, G = 1, B = 2, A = 3, CHANNEL_SIZE = 255;
    static const int R_SHIFT = 24,
                     G_SHIFT = 16,
                     B_SHIFT = 8,
                     A_SHIFT = 0;
    static const uint32_t R_BITS = 0xFF000000,
                          B_BITS = 0x00FF0000,
                          G_BITS = 0x0000FF00,
                          A_BITS = 0x000000FF;

    color();
    explicit color(float *c);
    color(color const &c);
    color(float r, float g, float b);
    color(float r, float g, float b, float a);
    color(uint32_t hex);
    color(std::string hex);
    virtual ~color();

    float r();
    float g();
    float b();
    float a();

    void from_hex(uint32_t hex);
    void from_hex(std::string hex);
    void set(float *c);
    void set(float r, float g, float b);
    void set(float r, float g, float b, float a);

    color *clamp();
    color *lerp(color *target, double a);

    color *mul(color *c);
    color *mul(double d);
    color *add(color *c);
    color *add(double d);
    color *sub(color *c);
    color *sub(double d);

    float *raw_data();


private:
    float data[4];
};

}

#endif //GLW_COLOR_HPP
