//
// Created by Garrett on 10/12/21.
//

#include "color.hpp"

namespace glw {

color::color() : color(1.f, 1.f, 1.f, 1.f) {}

color::color(float *c) : color(c[R], c[G], c[B], c[A]) {}

color::color(const color &c) :
    color(c.data[R], c.data[G], c.data[B], c.data[A]) {}

color::color(float r, float g, float b) : color(r, g, b, 1.f) {}

color::color(float r, float g, float b, float a)
{
    this->set(r, g, b, a);
}

color::color(uint32_t hex)
{
    this->from_hex(hex);
}

color::color(std::string hex)
{
    this->from_hex(hex);
}

color::~color() = default;

float color::r()
{
    return this->data[R];
}

float color::g()
{
    return this->data[G];
}

float color::b()
{
    return this->data[B];
}


float color::a()
{
    return this->data[A];
}

void color::from_hex(uint32_t hex)
{
    uint8_t r = (hex & R_BITS) >> R_SHIFT,
            g = (hex & G_BITS) >> G_SHIFT,
            b = (hex & B_BITS) >> B_SHIFT,
            a = (hex & A_BITS) >> A_SHIFT;

    this->set((float) r / CHANNEL_SIZE,
              (float) g / CHANNEL_SIZE,
              (float) b / CHANNEL_SIZE,
              (float) a / CHANNEL_SIZE);

}

void color::from_hex(std::string hex)
{
    return this->from_hex(std::stoul(hex));
}

void color::set(float *c)
{
    this->data[R] = c[R];
    this->data[G] = c[G];
    this->data[B] = c[B];
    this->data[A] = c[A];
}

void color::set(float r, float g, float b)
{
    this->data[R] = r;
    this->data[G] = g;
    this->data[B] = b;
    this->data[A] = 1.f;
}

void color::set(float r, float g, float b, float a)
{
    this->data[R] = r;
    this->data[G] = g;
    this->data[B] = b;
    this->data[A] = a;
}

color *color::clamp()
{
    if (this->data[R] < CHANNEL_MIN)
        this->data[R] = CHANNEL_MIN;
    else if (this->data[R] > CHANNEL_MAX)
        this->data[R] = CHANNEL_MAX;

    if (this->data[G] < CHANNEL_MIN)
        this->data[G] = CHANNEL_MIN;
    else if (this->data[G] > CHANNEL_MAX)
        this->data[G] = CHANNEL_MAX;

    if (this->data[B] < CHANNEL_MIN)
        this->data[B] = CHANNEL_MIN;
    else if (this->data[B] > CHANNEL_MAX)
        this->data[B] = CHANNEL_MAX;

    if (this->data[A] < CHANNEL_MIN)
        this->data[A] = CHANNEL_MIN;
    else if (this->data[A] > CHANNEL_MAX)
        this->data[A] = CHANNEL_MAX;

    return this;
}

color *color::lerp(color *target, double a)
{
    return nullptr;
}

color *color::mul(color *c)
{
    return nullptr;
}

color *color::mul(double d)
{
    return nullptr;
}

color *color::add(color *c)
{
    return nullptr;
}

color *color::add(double d)
{
    return nullptr;
}

color *color::sub(color *c)
{
    return nullptr;
}

color *color::sub(double d)
{
    return nullptr;
}

float *color::raw_data()
{
    return this->data;
}

}