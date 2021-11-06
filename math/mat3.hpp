//
// Created by Garrett on 9/24/21.
//

#ifndef GLW_MAT3_HPP
#define GLW_MAT3_HPP

#include "../util/exception.hpp"
#include "../util/math.hpp"

#include "mat.hpp"
#include "vec.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace glw {

template<typename T>
class mat3 : public mat<T> {
public:
    const int M11 = 0, M12 = 1, M13 = 2;
    const int M21 = 3, M22 = 4, M23 = 5;
    const int M31 = 6, M32 = 7, M33 = 8;

    static mat3<T> ZERO;
    static mat3<T> I;

    mat3();
    explicit mat3(T *e);
    explicit mat3(mat3<T> *m);
    mat3(vec3<T> *c1, vec3<T> *c2, vec3<T> *c3);
    ~mat3();

    int dim() override;
    int rows() override;
    int cols() override;

    mat<T> *set(T *e) override;
    mat<T> *set(T v) override;

    mat<T> *clone() override;
    mat<T> *copy(mat<T> *m) override;

    mat<T> *add(mat<T> *m) override;
    mat<T> *sub(mat<T> *m) override;

    mat<T> *s_mul(double s) override;
    mat<T> *l_mul(mat<T> *m) override;
    mat<T> *r_mul(mat<T> *m) override;
    vec<T> *v_mul(vec<T> *v) override;

    mat<T> *identity() override;

    mat3<T> *scale(T x, T y);
    mat3<T> *scale(vec2<T> *v);
    mat<T> *scale(T x, T y, T z) override;
    mat<T> *scale(vec3<T> *v) override;

    mat3<T> *set_scale(T x, T y);
    mat3<T> *set_scale(vec2<T> *v);
    mat<T> *set_scale(T x, T y, T z) override;
    mat<T> *set_scale(vec3<T> *v) override;

    mat<T> *rot_x(double deg) override;
    mat<T> *rot_x_r(double rad) override;
    mat<T> *rot_y(double deg) override;
    mat<T> *rot_y_r(double rad) override;
    mat<T> *rot_z(double deg) override;
    mat<T> *rot_z_r(double rad) override;

    mat<T> *set_rot_x(double deg) override;
    mat<T> *set_rot_x_r(double rad) override;
    mat<T> *set_rot_y(double deg) override;
    mat<T> *set_rot_y_r(double rad) override;
    mat<T> *set_rot_z(double deg) override;
    mat<T> *set_rot_z_r(double rad) override;

    mat3<T> *translate(T x, T y);
    mat3<T> *translate(vec2<T> *v);
    mat<T> *translate(T x, T y, T z) override;
    mat<T> *translate(vec3<T> *v) override;

    mat3<T> *set_translate(T x, T y);
    mat3<T> *set_translate(vec2<T> *v);
    mat<T> *set_translate(T x, T y, T z) override;
    mat<T> *set_translate(vec3<T> *v) override;

    double det() override;
    mat<T> *transpose() override;
    mat<T> *inverse() override;

    T *raw_data() override;

    T &operator[](int i);
    mat3<T> *operator+(mat3<T> *m);
    void operator+=(mat3<T> *m);
    mat3<T> *operator-(mat3<T> *m);
    void operator-=(mat3<T> *m);
    mat3<T> *operator*(double s);
    void operator*=(double s);
    mat3<T> *operator*(mat3<T> *m);
    bool operator==(mat3<T> *m) const;
    bool operator!=(mat3<T> *m) const;
    mat3<T> &operator=(mat3<T> *m);
    /*
     * inverse().
     */
    mat3<T> *operator!();
    /*
     * det().
     */
    double operator~();
    /*
     * transpose().
     */
    mat3<T> *operator-();

    std::string str() override;

private:
    const int ROWS = 3;
    const int COLS = ROWS;
    const int DIM = ROWS * COLS;
};

template<typename T>
mat3<T> mat3<T>::ZERO = mat3<T>();
template<typename T>
mat3<T> mat3<T>::I = mat3<T>().identity();

template<typename T>
mat3<T>::mat3()
{
    this->data = new T[DIM];

    this->data[M11] = (T) 0.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 0.0;
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 0.0;
}

template<typename T>
mat3<T>::mat3(mat3<T> *m)
{
    auto e = m->raw_data();
    this->data = new T[DIM];

    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];
}

template<typename T>
mat3<T>::mat3(T *e)
{
    this->data = new T[DIM];

    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];
}

template<typename T>
mat3<T>::mat3(vec3<T> *c1, vec3<T> *c2, vec3<T> *c3)
{
    this->data = new T[DIM];

    this->data[M11] = c1->x();
    this->data[M12] = c2->x();
    this->data[M13] = c3->x();

    this->data[M21] = c1->y();
    this->data[M22] = c2->y();
    this->data[M23] = c3->y();

    this->data[M31] = c1->z();
    this->data[M32] = c2->z();
    this->data[M33] = c3->z();
}

template<typename T>
mat3<T>::~mat3()
{
    delete [] this->data;
}

template<typename T>
int mat3<T>::dim()
{
    return DIM;
}

template<typename T>
int mat3<T>::rows()
{
    return ROWS;
}

template<typename T>
int mat3<T>::cols()
{
    return COLS;
}

template<typename T>
mat<T> *mat3<T>::set(T *e)
{
    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];

    return this;
}

template<typename T>
mat<T> *mat3<T>::set(T v)
{
    this->data[M11] = v;
    this->data[M12] = v;
    this->data[M13] = v;

    this->data[M21] = v;
    this->data[M22] = v;
    this->data[M23] = v;

    this->data[M31] = v;
    this->data[M32] = v;
    this->data[M33] = v;

    return this;
}

template<typename T>
mat<T> *mat3<T>::clone()
{
    return new mat3<T>(this);
}

template<typename T>
mat<T> *mat3<T>::copy(mat<T> *m)
{
    if (!dynamic_cast<mat3<T> *>(m))
        return nullptr;

    auto e = m->raw_data();

    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];

    return this;
}

template<typename T>
mat<T> *mat3<T>::add(mat<T> *m)
{
    if (!dynamic_cast<mat3<T> *>(m))
        return nullptr;

    auto e = m->raw_data();

    this->data[M11] += e[M11];
    this->data[M12] += e[M12];
    this->data[M13] += e[M13];

    this->data[M21] += e[M21];
    this->data[M22] += e[M22];
    this->data[M23] += e[M23];

    this->data[M31] += e[M31];
    this->data[M32] += e[M32];
    this->data[M33] += e[M33];

    return this;
}

template<typename T>
mat<T> *mat3<T>::sub(mat<T> *m)
{
    if (!dynamic_cast<mat3<T> *>(m))
        return nullptr;

    auto e = m->raw_data();

    this->data[M11] -= e[M11];
    this->data[M12] -= e[M12];
    this->data[M13] -= e[M13];

    this->data[M21] -= e[M21];
    this->data[M22] -= e[M22];
    this->data[M23] -= e[M23];

    this->data[M31] -= e[M31];
    this->data[M32] -= e[M32];
    this->data[M33] -= e[M33];

    return this;
}

template<typename T>
mat<T> *mat3<T>::s_mul(double s)
{
    this->data[M11] *= s;
    this->data[M12] *= s;
    this->data[M13] *= s;

    this->data[M21] *= s;
    this->data[M22] *= s;
    this->data[M23] *= s;

    this->data[M31] *= s;
    this->data[M32] *= s;
    this->data[M33] *= s;

    return this;
}

template<typename T>
mat<T> *mat3<T>::l_mul(mat<T> *m)
{
    if (!dynamic_cast<mat3<T> *>(m))
        return nullptr;

    auto e = this->data;
    auto o = m->raw_data();

    T t[this->DIM];

    t[M11] = e[M11] * o[M11] + e[M12] * o[M21] + e[M13] * o[M31];
    t[M12] = e[M11] * o[M12] + e[M12] * o[M22] + e[M13] * o[M32];
    t[M13] = e[M11] * o[M13] + e[M12] * o[M23] + e[M13] * o[M33];

    t[M21] = e[M21] * o[M11] + e[M22] * o[M21] + e[M23] * o[M31];
    t[M22] = e[M21] * o[M12] + e[M22] * o[M22] + e[M23] * o[M32];
    t[M23] = e[M21] * o[M13] + e[M22] * o[M23] + e[M23] * o[M33];

    t[M31] = e[M31] * o[M11] + e[M32] * o[M21] + e[M33] * o[M31];
    t[M32] = e[M31] * o[M12] + e[M32] * o[M22] + e[M33] * o[M32];
    t[M33] = e[M31] * o[M13] + e[M32] * o[M23] + e[M33] * o[M33];

    e[M11] = t[M11];
    e[M12] = t[M12];
    e[M13] = t[M13];

    e[M21] = t[M21];
    e[M22] = t[M22];
    e[M23] = t[M23];

    e[M31] = t[M31];
    e[M32] = t[M32];
    e[M33] = t[M33];

    return this;
}

template<typename T>
mat<T> *mat3<T>::r_mul(mat<T> *m)
{
    if (!dynamic_cast<mat3<T> *>(m))
        return nullptr;

    auto e = this->data;
    auto o = m->raw_data();

    T t[this->DIM];

    t[M11] = o[M11] * e[M11] + o[M12] * e[M21] + o[M13] * e[M31];
    t[M12] = o[M11] * e[M12] + o[M12] * e[M22] + o[M13] * e[M32];
    t[M13] = o[M11] * e[M13] + o[M12] * e[M23] + o[M13] * e[M33];

    t[M21] = o[M21] * e[M11] + o[M22] * e[M21] + o[M23] * e[M31];
    t[M22] = o[M21] * e[M12] + o[M22] * e[M22] + o[M23] * e[M32];
    t[M23] = o[M21] * e[M13] + o[M22] * e[M23] + o[M23] * e[M33];

    t[M31] = o[M31] * e[M11] + o[M32] * e[M21] + o[M33] * e[M31];
    t[M32] = o[M31] * e[M12] + o[M32] * e[M22] + o[M33] * e[M32];
    t[M33] = o[M31] * e[M13] + o[M32] * e[M23] + o[M33] * e[M33];

    e[M11] = t[M11];
    e[M12] = t[M12];
    e[M13] = t[M13];

    e[M21] = t[M21];
    e[M22] = t[M22];
    e[M23] = t[M23];

    e[M31] = t[M31];
    e[M32] = t[M32];
    e[M33] = t[M33];

    return this;
}

template<typename T>
vec<T> *mat3<T>::v_mul(vec<T> *v)
{
    auto _v = dynamic_cast<vec3<T> *>(v);
    if (!_v)
        return nullptr;

    auto e = this->data;

    return new vec3<T>(
            e[M11] * _v->x() + e[M12] * _v->y() + e[M13] * _v->z(),
            e[M21] * _v->x() + e[M22] * _v->y() + e[M23] * _v->z(),
            e[M31] * _v->x() + e[M32] * _v->y() + e[M33] * _v->z()
    );
}

template<typename T>
mat<T> *mat3<T>::identity()
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;

    return this;
}

template<typename T>
mat3<T> *mat3<T>::scale(T x, T y)
{
    auto tmp = mat3<T>().set_scale(x, y);

    return dynamic_cast<mat3<T> *>(this->l_mul(tmp));
}

template<typename T>
mat3<T> *mat3<T>::scale(vec2<T> *v)
{
    auto tmp = mat3<T>().set_scale(v);

    return dynamic_cast<mat3<T> *>(this->l_mul(tmp));
}

template<typename T>
mat<T> *mat3<T>::scale(T x, T y, T z)
{
    auto tmp = mat3<T>().set_scale(x, y, z);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat3<T>::scale(vec3<T> *v)
{
    auto tmp = mat3<T>().set_scale(v);

    return this->l_mul(tmp);
}

template<typename T>
mat3<T> *mat3<T>::set_scale(T x, T y)
{
    this->data[M11] = x;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = y;
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;

    return this;
}

template<typename T>
mat3<T> *mat3<T>::set_scale(vec2<T> *v)
{
    this->data[M11] = v->x();
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = v->y();
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat3<T>::set_scale(T x, T y, T z)
{
    this->data[M11] = x;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = y;
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = z;

    return this;
}

template<typename T>
mat<T> *mat3<T>::set_scale(vec3<T> *v)
{
    this->data[M11] = v->x();
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = v->y();
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = v->z();

    return this;
}

template<typename T>
mat<T> *mat3<T>::rot_x(double deg)
{
    return this->rot_x_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat3<T>::rot_x_r(double rad)
{
    auto tmp = mat3<T>().set_rot_x_r(rad);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat3<T>::rot_y(double deg)
{
    return this->rot_y_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat3<T>::rot_y_r(double rad)
{
    auto tmp = mat3<T>().set_rot_y_r(rad);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat3<T>::rot_z(double deg)
{
    return this->rot_z_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat3<T>::rot_z_r(double rad)
{
    auto tmp = mat3<T>().set_rot_z_r(rad);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat3<T>::set_rot_x(double deg)
{
    return this->set_rot_x_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat3<T>::set_rot_x_r(double rad)
{
    auto s = trig_round_err(std::sin(rad));
    auto c = trig_round_err(std::cos(rad));

    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) c;
    this->data[M23] = (T) -1.0 * s;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) s;
    this->data[M33] = (T) c;

    return this;
}

template<typename T>
mat<T> *mat3<T>::set_rot_y(double deg)
{
    return this->set_rot_y_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat3<T>::set_rot_y_r(double rad)
{
    auto s = trig_round_err(std::sin(rad));
    auto c = trig_round_err(std::cos(rad));

    this->data[M11] = (T) c;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) s;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) -1.0 * s;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) c;

    return this;
}

template<typename T>
mat<T> *mat3<T>::set_rot_z(double deg)
{
    return this->set_rot_z_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat3<T>::set_rot_z_r(double rad)
{
    auto s = trig_round_err(std::sin(rad));
    auto c = trig_round_err(std::cos(rad));

    this->data[M11] = (T) c;
    this->data[M12] = (T) -1.0 * s;
    this->data[M13] = (T) 0.0;

    this->data[M21] = (T) s;
    this->data[M22] = (T) c;
    this->data[M23] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;

    return this;
}

template<typename T>
mat3<T> *mat3<T>::translate(T x, T y)
{
    auto tmp = mat3<T>().set_translate(x, y);

    return dynamic_cast<mat3<T> *>(this->l_mul(tmp));
}

template<typename T>
mat3<T> *mat3<T>::translate(vec2<T> *v)
{
    auto tmp = mat3<T>().set_translate(v);

    return dynamic_cast<mat3<T> *>(this->l_mul(tmp));
}

template<typename T>
mat<T> *mat3<T>::translate(T x, T y, T z)
{
    auto tmp = mat3<T>().set_translate(x, y, z);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat3<T>::translate(vec3<T> *v)
{
    auto tmp = mat3<T>().set_translate(v);

    return this->l_mul(tmp);
}

template<typename T>
mat3<T> *mat3<T>::set_translate(T x, T y)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = x;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = y;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;

    return this;
}

template<typename T>
mat3<T> *mat3<T>::set_translate(vec2<T> *v)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = v->x();

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = v->y();

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat3<T>::set_translate(T x, T y, T z)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = x;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = y;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = z;

    return this;
}

template<typename T>
mat<T> *mat3<T>::set_translate(vec3<T> *v)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = v->x();

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = v->y();

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = v->z();

    return this;
}

template<typename T>
double mat3<T>::det()
{
    auto e = this->data;

    return (e[M11] * (e[M22] * e[M33] - e[M23] * e[M32])) -
           (e[M12] * (e[M21] * e[M33] - e[M23] * e[M31])) +
           (e[M13] * (e[M21] * e[M32] - e[M22] * e[M31]));
}

template<typename T>
mat<T> *mat3<T>::transpose()
{
    T tmp = this->data[M12];
    this->data[M12] = this->data[M21];
    this->data[M21] = tmp;

    tmp = this->data[M13];
    this->data[M13] = this->data[M31];
    this->data[M31] = tmp;

    tmp = this->data[M23];
    this->data[M23] = this->data[M32];
    this->data[M32] = tmp;

    return this;
}

template<typename T>
mat<T> *mat3<T>::inverse()
{
    double det = this->det();
    if (det == 0)
        throw div_by_zero_ex("Can't invert a singular mat3");

    double inv_det = 1.0 / det;
    T tmp[DIM];
    auto e = this->data;

    tmp[M11] = e[M22] * e[M33] - e[M23] * e[M32];
    tmp[M12] = e[M13] * e[M32] - e[M12] * e[M33];
    tmp[M13] = e[M12] * e[M23] - e[M13] * e[M22];

    tmp[M21] = e[M23] * e[M31] - e[M21] * e[M33];
    tmp[M22] = e[M11] * e[M33] - e[M13] * e[M31];
    tmp[M23] = e[M13] * e[M21] - e[M11] * e[M23];

    tmp[M31] = e[M21] * e[M32] - e[M22] * e[M31];
    tmp[M32] = e[M12] * e[M31] - e[M11] * e[M32];
    tmp[M33] = e[M11] * e[M22] - e[M12] * e[M21];

    e[M11] = inv_det * tmp[M11];
    e[M12] = inv_det * tmp[M12];
    e[M13] = inv_det * tmp[M13];

    e[M21] = inv_det * tmp[M21];
    e[M22] = inv_det * tmp[M22];
    e[M23] = inv_det * tmp[M23];

    e[M31] = inv_det * tmp[M31];
    e[M32] = inv_det * tmp[M32];
    e[M33] = inv_det * tmp[M33];

    return this;
}

template<typename T>
T *mat3<T>::raw_data()
{
    return this->data;
}

template<typename T>
T &mat3<T>::operator[](int i)
{
    if (i < 0 || i >= DIM)
        throw out_of_bounds_ex(
                "Index: " + std::to_string(i) + " out of bounds for mat3");

    return this->data[i];
}

template<typename T>
mat3<T> *mat3<T>::operator+(mat3<T> *m)
{
    return dynamic_cast<mat3<T> *>(this->add(m));
}

template<typename T>
void mat3<T>::operator+=(mat3<T> *m)
{
    this->add(m);
}

template<typename T>
mat3<T> *mat3<T>::operator-(mat3<T> *m)
{
    return dynamic_cast<mat3<T> *>(this->sub(m));
}

template<typename T>
void mat3<T>::operator-=(mat3<T> *m)
{
    this->sub(m);
}

template<typename T>
mat3<T> *mat3<T>::operator*(double s)
{
    return dynamic_cast<mat3<T> *>(this->s_mul(s));
}

template<typename T>
void mat3<T>::operator*=(double s)
{
    this->s_mul(s);
}

template<typename T>
mat3<T> *mat3<T>::operator*(mat3<T> *m)
{
    return dynamic_cast<mat3<T> *>(this->r_mul(m));
}

template<typename T>
bool mat3<T>::operator==(mat3<T> *m) const
{
    auto e = m->raw_data();

    for (auto i = 0; i < DIM; i++) {
        if (this->data[i] != e[i])
            return false;
    }

    return true;
}

template<typename T>
bool mat3<T>::operator!=(mat3<T> *m) const
{
    return !(*this == m);
}

template<typename T>
mat3<T> &mat3<T>::operator=(mat3<T> *m)
{
    (void) this->copy(m);

    return *this;
}

template<typename T>
mat3<T> *mat3<T>::operator!()
{
    return dynamic_cast<mat3<T> *>(this->inverse());
}

template<typename T>
double mat3<T>::operator~()
{
    return this->det();
}

template<typename T>
mat3<T> *mat3<T>::operator-()
{
    return dynamic_cast<mat3<T> *>(this->transpose());
}

template<typename T>
std::string mat3<T>::str()
{
    return "[" + std::to_string(this->data[M11]) + ", " +
                 std::to_string(this->data[M12]) + ", " +
                 std::to_string(this->data[M13]) + "]\n" +
           "[" + std::to_string(this->data[M21]) + ", " +
                 std::to_string(this->data[M22]) + ", " +
                 std::to_string(this->data[M23]) + "]\n" +
           "[" + std::to_string(this->data[M31]) + ", " +
                 std::to_string(this->data[M32]) + ", " +
                 std::to_string(this->data[M33]) + "]";
}

}

#endif //GLW_MAT3_HPP
