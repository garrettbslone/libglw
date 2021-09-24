//
// Created by Garrett on 9/13/21.
//

#ifndef LIBGLW_VEC4_HPP
#define LIBGLW_VEC4_HPP

#include <cmath>
#include <string>

#include "../util/exception.hpp"
#include "vec.hpp"

namespace glw {

template<typename T>
class vec4 : public vec<T> {
public:
    static vec4<T> ZERO;
    static vec4<T> X;
    static vec4<T> Y;
    static vec4<T> Z;
    static vec4<T> W;

    const int _dim = 4;
    int dim() override { return _dim; }

    vec4();
    explicit vec4(vec4 *);
    vec4(T x, T y, T z, T w);
    ~vec4();

    vec<T> *set(T x, T y, T z, T w);
    vec<T> *clone() override;
    vec<T> *copy(vec<T> *v) override;

    vec<T> *add(vec<T> *v) override;
    vec<T> *sub(vec<T> *v) override;
    vec<T> *scale(double s) override;
    vec<T> *negate() override;

    double dot(vec<T> *v) override;
    double mag() override;
    double mag2() override;

    static vec4<T> *from_to(vec4<T> *from, vec4<T> *to);
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
    T z() { return this->data[2]; };
    T w() { return this->data[3]; };

    T &operator[](int i);
    vec4<T> *operator+(vec4<T> *v);
    void operator+=(vec4<T> *v);
    vec4<T> *operator-(vec4<T> *v);
    void operator-=(vec4<T> *v);
    vec4<T> *operator*(double s);
    void operator*=(double s);
    double operator*(vec4<T> *v);
    bool operator==(vec4<T> *v) const;
    bool operator!=(vec4<T> *v) const;

    std::string str() override;
};

template<typename T>
vec4<T> vec4<T>::ZERO = vec4<T>((T) 0.0, (T) 0.0, (T) 0.0, (T) 0.0);
template<typename T>
vec4<T> vec4<T>::X = vec4<T>((T) 1.0, (T) 0.0, (T) 0.0, (T) 0.0);
template<typename T>
vec4<T> vec4<T>::Y = vec4<T>((T) 0.0, (T) 1.0, (T) 0.0, (T) 0.0);
template<typename T>
vec4<T> vec4<T>::Z = vec4<T>((T) 0.0, (T) 0.0, (T) 1.0, (T) 0.0);
template<typename T>
vec4<T> vec4<T>::W = vec4<T>((T) 0.0, (T) 0.0, (T) 0.0, (T) 1.0);

template<typename T>
vec4<T>::vec4()
{
    this->data = new T[4];

    this->data[0] = (T) 0.0;
    this->data[1] = (T) 0.0;
    this->data[2] = (T) 0.0;
    this->data[3] = (T) 0.0;
}

template<typename T>
vec4<T>::vec4(vec4 *v)
{
    this->data = new T[4];

    this->data[0] = v->x();
    this->data[1] = v->y();
    this->data[2] = v->z();
    this->data[3] = v->w();
}

template<typename T>
vec4<T>::vec4(T x, T y, T z, T w)
{
    this->data = new T[4];

    this->data[0] = x;
    this->data[1] = y;
    this->data[2] = z;
    this->data[3] = w;
}

template<typename T>
vec4<T>::~vec4()
{
    delete [] this->data;
}

template<typename T>
vec<T> *vec4<T>::set(T x, T y, T z, T w)
{
    this->data[0] = x;
    this->data[1] = y;
    this->data[2] = z;
    this->data[3] = w;

    return this;
}

template<typename T>
vec<T> *vec4<T>::clone()
{
    return new vec4<T>(this);
}

template<typename T>
vec<T> *vec4<T>::copy(vec<T>* v)
{
    auto _v = dynamic_cast<vec4<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] = _v->x();
    this->data[1] = _v->y();
    this->data[2] = _v->z();
    this->data[3] = _v->w();

    return this;
}

template<typename T>
vec<T> *vec4<T>::add(vec<T> *v)
{
    auto _v = dynamic_cast<vec4<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] += _v->x();
    this->data[1] += _v->y();
    this->data[2] += _v->z();
    this->data[3] += _v->w();

    return this;
}

template<typename T>
vec<T> *vec4<T>::sub(vec<T> *v)
{
    auto _v = dynamic_cast<vec4<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] -= _v->x();
    this->data[1] -= _v->y();
    this->data[2] -= _v->z();
    this->data[3] -= _v->w();

    return this;
}

template<typename T>
vec<T> *vec4<T>::scale(double s)
{
    this->data[0] *= s;
    this->data[1] *= s;
    this->data[2] *= s;
    this->data[3] *= s;

    return this;
}

template<typename T>
vec<T> *vec4<T>::negate()
{
    this->data[0] *= -1;
    this->data[1] *= -1;
    this->data[2] *= -1;
    this->data[3] *= -1;

    return this;
}

template<typename T>
double vec4<T>::dot(vec<T> *v)
{
    auto _v = dynamic_cast<vec4<T> *>(v);

    if (!_v)
        return 0.0;

    return this->data[0] * _v->x() +
           this->data[1] * _v->y() +
           this->data[2] * _v->z() +
           this->data[3] * _v->w();
}

template<typename T>
double vec4<T>::mag()
{
    return sqrt(this->data[0] * this->data[0] +
                this->data[1] * this->data[1] +
                this->data[2] * this->data[2] +
                this->data[3] * this->data[3]);
}

template<typename T>
double vec4<T>::mag2()
{
    return this->data[0] * this->data[0] +
           this->data[1] * this->data[1] +
           this->data[2] * this->data[2] +
           this->data[3] * this->data[3];
}

template<typename T>
vec4<T> *vec4<T>::from_to(vec4<T> *from, vec4<T> *to)
{
    auto _from = dynamic_cast<vec4<T> *>(from);
    auto _to = dynamic_cast<vec4<T> *>(to);

    if (!_from || _to)
        return nullptr;

    return new vec4(_to->x() - _from->x(),
                    _to->y() - _from->y(),
                    _to->z() - _from->z(),
                    _to->w() - _from->w());
}

template<typename T>
vec<T> *vec4<T>::project(vec<T> *onto)
{
    auto _onto = dynamic_cast<vec4<T> *>(onto);

    if (!_onto)
        return nullptr;

    auto tmp = this->data;
    this->data = _onto->scale(this->dot(_onto) / _onto->mag2())->raw_data();
    delete [] tmp;

    return this;
}

template<typename T>
vec<T> *vec4<T>::normalize()
{
    if (this->is_zero())
        throw div_by_zero_ex("vec4 with magnitude of 0 cannot be normalized.");

    this->scale(1 / this->mag());
    return this;
}

template<typename T>
vec<T> *vec4<T>::lerp(vec<T> *target, double a)
{
    auto _target = dynamic_cast<vec4<T> *>(target);

    if (!_target)
        return nullptr;

    this->data[0] += a * (_target->x() - this->data[0]);
    this->data[1] += a * (_target->y() - this->data[1]);
    this->data[2] += a * (_target->z() - this->data[2]);
    this->data[3] += a * (_target->w() - this->data[3]);

    return this;
}

template<typename T>
double vec4<T>::dist(vec<T> *to)
{
    return sqrt(this->dist2(to));
}

template<typename T>
double vec4<T>::dist2(vec<T> *to)
{
    auto _to = dynamic_cast<vec4<T> *>(to);

    if (!_to)
        return 0.0;

    double x = this->data[0] - _to->x();
    double y = this->data[1] - _to->y();
    double z = this->data[2] - _to->z();
    double w = this->data[3] - _to->w();

    return x * x + y * y + z * z + w * w;
}

template<typename T>
bool vec4<T>::is_unit()
{
    return this->mag() == 1.0;
}

template<typename T>
bool vec4<T>::is_zero()
{
    return this->data[0] == (T) 0.0 &&
           this->data[1] == (T) 0.0 &&
           this->data[2] == (T) 0.0 &&
           this->data[3] == (T) 0.0;
}

template<typename T>
bool vec4<T>::is_collinear(vec<T> *v)
{
    // not implemented
    return false;
}

template<typename T>
bool vec4<T>::is_perpendicular(vec<T> *v)
{
    auto _v = dynamic_cast<vec4 *>(v);

    if (!_v)
        return false;

    return *this * _v == 0;
}

template<typename T>
bool vec4<T>::is_same_dir(vec<T>* v)
{
    auto _v = dynamic_cast<vec4 *>(v);

    if (!_v)
        return false;

    return *this * _v > 0;
}

template<typename T>
T *vec4<T>::raw_data()
{
    return this->data;
}

template<typename T>
T &vec4<T>::operator[](int i)
{
    if (i < 0 || i > this->dim)
        throw out_of_bounds_ex(
                "Index: " + std::to_string(i) + " out of bounds for vec3");

    return this->data[i];
}

template<typename T>
vec4<T> *vec4<T>::operator+(vec4<T> *v)
{
    return dynamic_cast<vec4<T> *>(this->add(v));
}

template<typename T>
void vec4<T>::operator+=(vec4<T> *v)
{
    this->add(v);
}

template<typename T>
vec4<T> *vec4<T>::operator-(vec4<T> *v)
{
    return dynamic_cast<vec4<T> *>(this->sub(v));
}

template<typename T>
void vec4<T>::operator-=(vec4<T> *v)
{
    this->sub(v);
}

template<typename T>
vec4<T> *vec4<T>::operator*(double s)
{
    return dynamic_cast<vec4<T> *>(this->scale(s));
}

template<typename T>
void vec4<T>::operator*=(double s)
{
    this->scale(s);
}

template<typename T>
double vec4<T>::operator*(vec4<T> *v)
{
    return this->dot(v);
}

template<typename T>
std::string vec4<T>::str()
{
    return "[" + std::to_string(this->x()) + ", " +
                 std::to_string(this->y()) + ", " +
                 std::to_string(this->z()) + ", " +
                 std::to_string(this->w()) + "]";
}
template<typename T>
bool vec4<T>::operator==(vec4<T> *v) const
{
    return this->data[0] == v->x() &&
           this->data[1] == v->y() &&
           this->data[2] == v->z() &&
           this->data[3] == v->w();
}
template<typename T>
bool vec4<T>::operator!=(vec4<T> *v) const
{
    return !(*this == v);
}

}

#endif //LIBGLW_VEC4_HPP
