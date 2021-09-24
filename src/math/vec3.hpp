//
// Created by Garrett on 9/13/21.
//

#ifndef LIBGLW_VEC3_HPP
#define LIBGLW_VEC3_HPP

#include <cmath>
#include <string>

#include "../util/exception.hpp"
#include "vec.hpp"

namespace glw {

template<typename T>
class vec3 : public vec<T> {
public:
    static vec3<T> ZERO;
    static vec3<T> X;
    static vec3<T> Y;
    static vec3<T> Z;

    const int _dim = 3;
    int dim() override { return _dim; }

    vec3();
    explicit vec3(vec3 *);
    vec3(T x, T y, T z);
    ~vec3();

    vec<T> *set(T x, T y, T z);
    vec<T> *clone() override;
    vec<T> *copy(vec<T> *v) override;

    vec<T> *add(vec<T> *v) override;
    vec<T> *sub(vec<T> *v) override;
    vec<T> *scale(double s) override;
    vec<T>* negate() override;

    vec3<T> *cross(vec3<T> *v);
    double dot(vec<T>* v) override;
    double mag() override;
    double mag2() override;

    static vec3<T> *from_to(vec3<T> *from, vec3<T> *to);
    vec<T> *project(vec<T> *onto) override;
    vec<T> *normalize() override;
    vec<T>* lerp(vec<T> *target, double a) override;

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

    T &operator[](int i);
    vec3<T> *operator+(vec3<T> *v);
    void operator+=(vec3<T> *v);
    vec3<T> *operator-(vec3<T> *v);
    void operator-=(vec3<T> *v);
    vec3<T> *operator*(double s);
    void operator*=(double s);
    double operator*(vec3<T> *v);
    vec3<T> *operator/(vec3<T> *v);
    bool operator==(vec3<T> *v) const;
    bool operator!=(vec3<T> *v) const;

    std::string str() override;
};

template<typename T>
vec3<T> vec3<T>::ZERO = vec3<T>((T) 0.0, (T) 0.0, (T) 0.0);
template<typename T>
vec3<T> vec3<T>::X = vec3<T>((T) 1.0, (T) 0.0, (T) 0.0);
template<typename T>
vec3<T> vec3<T>::Y = vec3<T>((T) 0.0, (T) 1.0, (T) 0.0);
template<typename T>
vec3<T> vec3<T>::Z = vec3<T>((T) 0.0, (T) 0.0, (T) 1.0);

template<typename T>
vec3<T>::vec3()
{
    this->data = new T[3];

    this->data[0] = (T) 0.0;
    this->data[1] = (T) 0.0;
    this->data[2] = (T) 0.0;
}

template<typename T>
vec3<T>::vec3(vec3 *v)
{
    this->data = new T[3];

    this->data[0] = v->x();
    this->data[1] = v->y();
    this->data[2] = v->z();
}

template<typename T>
vec3<T>::vec3(T x, T y, T z)
{
    this->data = new T[3];

    this->data[0] = x;
    this->data[1] = y;
    this->data[2] = z;
}

template<typename T>
vec3<T>::~vec3()
{
    delete [] this->data;
}

template<typename T>
vec<T> *vec3<T>::set(T x, T y, T z)
{
    this->data[0] = x;
    this->data[1] = y;
    this->data[2] = z;

    return this;
}

template<typename T>
vec<T> *vec3<T>::clone()
{
    return new vec3<T>(this);
}

template<typename T>
vec<T> *vec3<T>::copy(vec<T> *v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] = _v->x();
    this->data[1] = _v->y();
    this->data[2] = _v->z();

    return this;
}

template<typename T>
vec<T> *vec3<T>::add(vec<T> *v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] += _v->x();
    this->data[1] += _v->y();
    this->data[2] += _v->z();

    return this;
}

template<typename T>
vec<T> *vec3<T>::sub(vec<T> *v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);

    if (!_v)
        return nullptr;

    this->data[0] -= _v->x();
    this->data[1] -= _v->y();
    this->data[2] -= _v->z();

    return this;
}

template<typename T>
vec<T> *vec3<T>::scale(double s)
{
    this->data[0] *= s;
    this->data[1] *= s;
    this->data[2] *= s;

    return this;
}

template<typename T>
vec<T> *vec3<T>::negate()
{
    this->data[0] *= -1;
    this->data[1] *= -1;
    this->data[2] *= -1;

    return this;
}

template<typename T>
vec3<T> *vec3<T>::cross(vec3<T> *v)
{
    return new vec3(v->y() * this->data[2] -  v->z() * this->data[1],
                    v->z() * this->data[0] -  v->x() * this->data[2],
                    v->x() * this->data[1] -  v->y() * this->data[0]);
}

template<typename T>
double vec3<T>::dot(vec<T> *v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);

    if (!_v)
        return 0.0;

    return this->data[0] * _v->x() +
           this->data[1] * _v->y() +
           this->data[2] * _v->z();
}

template<typename T>
double vec3<T>::mag()
{
    return sqrt(this->data[0] * this->data[0] +
                this->data[1] * this->data[1] +
                this->data[2] * this->data[2]);
}

template<typename T>
double vec3<T>::mag2()
{
    return this->data[0] * this->data[0] +
           this->data[1] * this->data[1] +
           this->data[2] * this->data[2];
}

template<typename T>
vec3<T> *vec3<T>::from_to(vec3<T> *from, vec3<T> *to)
{
    auto _from = dynamic_cast<vec3<T> *>(from);
    auto _to = dynamic_cast<vec3<T> *>(to);

    if (!_from || !_to)
        return nullptr;

    return new vec3(_to->x() - _from->x(),
                    _to->y() - _from->y(),
                    _to->z() - _from->z());
}

template<typename T>
vec<T> *vec3<T>::project(vec<T> *onto)
{
    auto _onto = dynamic_cast<vec3<T> *>(onto);

    if (!_onto)
        return nullptr;

    auto tmp = this->data;
    this->data = _onto->scale(this->dot(_onto) / _onto->mag2())->raw_data();
    delete [] tmp;

    return this;
}

template<typename T>
vec<T> *vec3<T>::normalize()
{
    if (this->is_zero())
        throw div_by_zero_ex("vec4 with magnitude of 0 cannot be normalized.");

    this->scale(1 / this->mag());
    return this;
}

template<typename T>
vec<T> *vec3<T>::lerp(vec<T> *target, double a)
{
    auto _target = dynamic_cast<vec3<T> *>(target);

    if (!_target)
        return nullptr;

    this->data[0] += a * (_target->x() - this->data[0]);
    this->data[1] += a * (_target->y() - this->data[1]);
    this->data[2] += a * (_target->z() - this->data[2]);

    return this;
}

template<typename T>
double vec3<T>::dist(vec<T> *to)
{
    return sqrt(this->dist2(to));
}

template<typename T>
double vec3<T>::dist2(vec<T> *to)
{
    auto _to = dynamic_cast<vec3<T> *>(to);

    if (!_to)
        return 0.0;

    double x = this->data[0] - _to->x();
    double y = this->data[1] - _to->y();
    double z = this->data[2] - _to->z();

    return x * x + y * y + z * z;
}

template<typename T>
bool vec3<T>::is_unit()
{
    return this->mag() == (T) 1.0;
}

template<typename T>
bool vec3<T>::is_zero()
{
    return this->data[0] == (T) 0.0 &&
           this->data[1] == (T) 0.0 &&
           this->data[2] == (T) 0.0;
}

template<typename T>
bool vec3<T>::is_collinear(vec<T> *v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);

    if (!_v)
        return false;

    return this->cross(_v)->mag2() <= 0.0000001;
}

template<typename T>
bool vec3<T>::is_perpendicular(vec<T> *v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);

    if (!_v)
        return false;

    return *this * _v == 0;
}

template<typename T>
bool vec3<T>::is_same_dir(vec<T>* v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);

    if (!_v)
        return false;

    return *this * _v > 0;
}

template<typename T>
T *vec3<T>::raw_data()
{
    return this->data;
}

template<typename T>
T &vec3<T>::operator[](int i)
{
    if (i < 0 || i > this->dim)
        throw out_of_bounds_ex(
                "Index: " + std::to_string(i) + " out of bounds for vec3");

    return this->data[i];
}

template<typename T>
vec3<T> *vec3<T>::operator+(vec3<T> *v)
{
    return dynamic_cast<vec3<T> *>(this->add(v));
}

template<typename T>
void vec3<T>::operator+=(vec3<T> *v)
{
    this->add(v);
}

template<typename T>
vec3<T> *vec3<T>::operator-(vec3<T> *v)
{
    return dynamic_cast<vec3<T> *>(this->sub(v));
}

template<typename T>
void vec3<T>::operator-=(vec3<T> *v)
{
    this->sub(v);
}

template<typename T>
vec3<T> *vec3<T>::operator*(double s)
{
    return dynamic_cast<vec3<T> *>(this->scale(s));
}

template<typename T>
void vec3<T>::operator*=(double s)
{
    this->scale(s);
}

template<typename T>
double vec3<T>::operator*(vec3<T> *v)
{
    return this->dot(v);
}

template<typename T>
vec3<T> *vec3<T>::operator/(vec3<T> *v)
{
    return this->cross(v);
}

template<typename T>
std::string vec3<T>::str()
{
    return "[" + std::to_string(this->x()) + ", " +
                 std::to_string(this->y()) + ", " +
                 std::to_string(this->z()) + "]";
}
template<typename T>
bool vec3<T>::operator==(vec3<T> *v) const
{
    return this->data[0] == v->x() &&
           this->data[1] == v->y() &&
           this->data[2] == v->z();
}
template<typename T>
bool vec3<T>::operator!=(vec3<T> *v) const
{
    return !(*this == v);
}

}

#endif //LIBGLW_VEC3_HPP
