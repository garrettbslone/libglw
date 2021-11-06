//
// Created by Garrett on 9/13/21.
//

#ifndef LIBGLW_VEC2_HPP
#define LIBGLW_VEC2_HPP

#include <cmath>
#include <string>

#include "../util/exception.hpp"
#include "vec.hpp"

namespace glw {

template<typename T>
class vec2 : public vec<T> {
public:
    static vec2<T> ZERO;
    static vec2<T> X;
    static vec2<T> Y;

    int dim() override { return DIM; }

    vec2();
    explicit vec2(vec2 *);
    vec2(T x, T y);
    ~vec2();

    vec<T> *set(T x, T y);
    vec<T> *clone() override;
    vec<T> *copy(vec<T> *v) override;

    vec<T> *add(vec<T> *v) override;
    vec<T> *sub(vec<T> *v) override;
    vec<T> *scale(double s) override;
    vec<T> *negate() override;

    double dot(vec<T> *v) override;
    double mag() override;
    double mag2() override;

    static vec2<T> *from_to(vec2<T> *from, vec2<T> *to);
    vec<T> *project(vec<T> *onto) override;
    vec<T> *normalize() override;
    vec<T> *lerp(vec<T> *target, double a) override;

    double dist(vec<T> *to) override;
    double dist2(vec<T> *to) override;

    bool is_unit() override;
    bool is_zero() override;
    bool is_collinear(vec<T> *v) override;
    bool is_perpendicular(vec<T> *v) override;
    bool is_same_dir(vec<T> *v) override;

    T *raw_data() override;
    T x() { return this->data[0]; };
    T y() { return this->data[1]; };

    T &operator[](int i);
    vec2<T> *operator+(vec2<T> *v);
    void operator+=(vec2<T> *v);
    vec2<T> *operator-(vec2<T> *v);
    void operator-=(vec2<T> *v);
    vec2<T> *operator*(double s);
    void operator*=(double s);
    double operator*(vec2<T> *v);
    bool operator==(vec2<T> *v) const;
    bool operator!=(vec2<T> *v) const;

    std::string str() override;

private:
    const int DIM = 2;
};

template<typename T>
vec2<T> vec2<T>::ZERO = vec2<T>((T) 0.0, (T) 0.0);
template<typename T>
vec2<T> vec2<T>::X = vec2<T>((T) 1.0, (T) 0.0);
template<typename T>
vec2<T> vec2<T>::Y = vec2<T>((T) 0.0, (T) 1.0);

template<typename T>
vec2<T>::vec2()
{
    this->data = new T[2];

    this->data[0] = (T) 0.0;
    this->data[1] = (T) 0.0;
}

template<typename T>
vec2<T>::vec2(vec2 *v)
{
    this->data = new T[2];

    this->data[0] = v->x();
    this->data[1] = v->y();
}

template<typename T>
vec2<T>::vec2(T x, T y)
{
    this->data = new T[2];

    this->data[0] = x;
    this->data[1] = y;
}

template<typename T>
vec2<T>::~vec2()
{
    delete [] this->data;
}

template<typename T>
vec<T> *vec2<T>::set(T x, T y)
{
    this->data[0] = x;
    this->data[1] = y;

    return this;
}

template<typename T>
vec<T> *vec2<T>::clone()
{
    return new vec2<T>(this);
}

template<typename T>
vec<T> *vec2<T>::copy(vec<T>* v)
{
    auto _v = dynamic_cast<vec2<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] = _v->x();
    this->data[1] = _v->y();

    return this;
}

template<typename T>
vec<T> *vec2<T>::add(vec<T> *v)
{
    auto _v = dynamic_cast<vec2<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] += _v->x();
    this->data[1] += _v->y();

    return this;
}

template<typename T>
vec<T> *vec2<T>::sub(vec<T> *v)
{
    auto _v = dynamic_cast<vec2<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] -= _v->x();
    this->data[1] -= _v->y();

    return this;
}

template<typename T>
vec<T> *vec2<T>::scale(double s)
{
    this->data[0] *= s;
    this->data[1] *= s;

    return this;
}

template<typename T>
vec<T> *vec2<T>::negate()
{
    this->data[0] *= -1;
    this->data[1] *= -1;

    return this;
}

template<typename T>
double vec2<T>::dot(vec<T> *v)
{
    auto _v = dynamic_cast<vec2<T> *>(v);

    if (!_v)
        return 0.0;

    return this->data[0] * _v->x() + this->data[1] * _v->y();
}

template<typename T>
double vec2<T>::mag()
{
    return sqrt(this->data[0] * this->data[0] + this->data[1] * this->data[1]);
}

template<typename T>
double vec2<T>::mag2()
{
    return this->data[0] * this->data[0] + this->data[1] * this->data[1];
}

template<typename T>
vec2<T> *vec2<T>::from_to(vec2<T> *from, vec2<T> *to)
{
    auto _from = dynamic_cast<vec2<T> *>(from);
    auto _to = dynamic_cast<vec2<T> *>(to);

    if (!_from || _to)
        return nullptr;

    return new vec2(_to->x() - _from->x(), _to->y() - _from->y());
}

template<typename T>
vec<T> *vec2<T>::project(vec<T> *onto)
{
    auto _onto = dynamic_cast<vec2<T> *>(onto);

    if (!_onto)
        return nullptr;

    auto tmp = this->data;
    this->data = _onto->scale(this->dot(_onto) / _onto->mag2())->raw_data();
    delete [] tmp;

    return this;
}

template<typename T>
vec<T> *vec2<T>::normalize()
{
    if (this->is_zero())
        throw div_by_zero_ex("vec2 with magnitude of 0 cannot be normalized.");

    this->scale(1 / this->mag());
    return this;
}

template<typename T>
vec<T> *vec2<T>::lerp(vec<T> *target, double a)
{
    auto _target = dynamic_cast<vec2<T> *>(target);

    if (!_target)
        return nullptr;

    this->data[0] += a * (_target->x() - this->data[0]);
    this->data[1] += a * (_target->y() - this->data[1]);

    return this;
}

template<typename T>
double vec2<T>::dist(vec<T> *to)
{
    return sqrt(this->dist2(to));
}

template<typename T>
double vec2<T>::dist2(vec<T> *to)
{
    auto _to = dynamic_cast<vec2<T> *>(to);

    if (!_to)
        return 0.0;

    double x = this->data[0] - _to->x();
    double y = this->data[1] - _to->y();

    return x * x + y * y;
}

template<typename T>
bool vec2<T>::is_unit()
{
    return this->mag() == 1.0;
}

template<typename T>
bool vec2<T>::is_zero()
{
    return this->data[0] == (T) 0.0 && this->data[1] == (T) 0.0;
}

template<typename T>
bool vec2<T>::is_collinear(vec<T> *v)
{
    auto _v = dynamic_cast<vec2 *>(v);

    if (!_v)
        return false;

    return (this->data[0] * _v->y() - this->data[1] * _v->x()) <= 0.0000001 &&
           this->dot(_v) > 0.0;

}

template<typename T>
bool vec2<T>::is_perpendicular(vec<T> *v)
{
    auto _v = dynamic_cast<vec2 *>(v);

    if (!_v)
        return false;

    return *this * _v == 0;
}

template<typename T>
bool vec2<T>::is_same_dir(vec<T>* v)
{
    auto _v = dynamic_cast<vec2 *>(v);

    if (!_v)
        return false;

    return *this * _v > 0;
}

template<typename T>
T *vec2<T>::raw_data()
{
    return this->data;
}

template<typename T>
T &vec2<T>::operator[](int i)
{
    if (i < 0 || i >= this->DIM)
        throw out_of_bounds_ex(
                "Index: " + std::to_string(i) + " out of bounds for vec3");

    return this->data[i];
}

template<typename T>
vec2<T> *vec2<T>::operator+(vec2<T> *v)
{
    return dynamic_cast<vec2<T> *>(this->add(v));
}

template<typename T>
void vec2<T>::operator+=(vec2<T> *v)
{
    this->add(v);
}

template<typename T>
vec2<T> *vec2<T>::operator-(vec2<T> *v)
{
    return dynamic_cast<vec2<T> *>(this->sub(v));
}

template<typename T>
void vec2<T>::operator-=(vec2<T> *v)
{
    this->sub(v);
}

template<typename T>
vec2<T> *vec2<T>::operator*(double s)
{
    return dynamic_cast<vec2<T> *>(this->scale(s));
}

template<typename T>
void vec2<T>::operator*=(double s)
{
    this->scale(s);
}

template<typename T>
double vec2<T>::operator*(vec2<T> *v)
{
    return this->dot(v);
}

template<typename T>
std::string vec2<T>::str()
{
    return "[" + std::to_string(this->x()) + ", " +
                 std::to_string(this->y()) + "]";
}
template<typename T>
bool vec2<T>::operator==(vec2<T> *v) const
{
    return this->data[0] == v->x() && this->data[1] == v->y();
}
template<typename T>
bool vec2<T>::operator!=(vec2<T> *v) const
{
    return !(*this == v);
}

}

#endif //LIBGLW_VEC2_HPP
