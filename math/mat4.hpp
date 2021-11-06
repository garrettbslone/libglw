//
// Created by Garrett on 9/26/21.
//

#ifndef GLW_MAT4_HPP
#define GLW_MAT4_HPP

#include "../util/exception.hpp"
#include "../util/math.hpp"

#include "mat.hpp"
#include "mat3.hpp"
#include "vec.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace glw {

template<typename T>
class mat4 : public mat<T> {
public:
    const int M11 = 0, M12 = 1, M13 = 2, M14 = 3;
    const int M21 = 4, M22 = 5, M23 = 6, M24 = 7;
    const int M31 = 8, M32 = 9, M33 = 10, M34 = 11;
    const int M41 = 12, M42 = 13, M43 = 14, M44 = 15;

    static mat4<T> ZERO;
    static mat4<T> I;

    mat4();
    explicit mat4(T *e);
    explicit mat4(mat3<T> *m);
    explicit mat4(mat4<T> *m);
    mat4(vec4<T> *c1, vec4<T> *c2, vec4<T> *c3, vec4<T> *c4);
    mat4(vec3<T> *c1, vec3<T> *c2, vec3<T> *c3);
    ~mat4();

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

    mat4<T> *scale(vec4<T> *v);
    mat<T> *scale(T x, T y, T z) override;
    mat<T> *scale(vec3<T> *v) override;

    mat4<T> *set_scale(vec4<T> *v);
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

    mat4<T> *translate(T x, T y, T z, T w);
    mat4<T> *translate(vec4<T> *v);
    mat<T> *translate(T x, T y, T z) override;
    mat<T> *translate(vec3<T> *v) override;

    mat4<T> *set_translate(T x, T y, T z, T w);
    mat4<T> *set_translate(vec4<T> *v);
    mat<T> *set_translate(T x, T y, T z) override;
    mat<T> *set_translate(vec3<T> *v) override;

    /*
     * Creates a perspective projection on this mat. fov_y should be in degrees.
     */
    mat4<T> *perspective(double fov_y,
                         double aspect,
                         double near,
                         double far);
    /*
     * Creates an orthographic projection on this mat.
     */
    mat4<T> *orthographic(double left,
                         double right,
                         double top,
                         double bottom,
                         double near,
                         double far);

    double det() override;
    mat<T> *transpose() override;
    mat<T> *inverse() override;

    T *raw_data() override;

    T &operator[](int i);
    mat4<T> *operator+(mat4<T> *m);
    void operator+=(mat4<T> *m);
    mat4<T> *operator-(mat4<T> *m);
    void operator-=(mat4<T> *m);
    mat4<T> *operator*(double s);
    void operator*=(double s);
    mat4<T> *operator*(mat4<T> *m);
    bool operator==(mat4<T> *m) const;
    bool operator!=(mat4<T> *m) const;
    mat4<T> &operator=(mat4<T> *m);
    /*
     * inverse().
     */
    mat4<T> *operator!();
    /*
     * det().
     */
    double operator~();
    /*
     * transpose().
     */
    mat4<T> *operator-();

    std::string str() override;

private:
    const int ROWS = 4;
    const int COLS = ROWS;
    const int DIM = ROWS * COLS;
};

template<typename T>
mat4<T> mat4<T>::ZERO = mat4<T>();
template<typename T>
mat4<T> mat4<T>::I = mat4<T>().identity();

template<typename T>
mat4<T>::mat4()
{
    this->data = new T[DIM];

    this->data[M11] = (T) 0.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 0.0;
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 0.0;
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 0.0;
}

template<typename T>
mat4<T>::mat4(T *e)
{
    this->data = new T[DIM];

    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];
    this->data[M14] = e[M14];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];
    this->data[M24] = e[M24];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];
    this->data[M34] = e[M34];

    this->data[M41] = e[M41];
    this->data[M42] = e[M42];
    this->data[M43] = e[M43];
    this->data[M44] = e[M44];
}

template<typename T>
mat4<T>::mat4(mat3<T> *m)
{
    auto e = m->raw_data();
    this->data = new T[DIM];

    this->data[M11] = e[m->M11];
    this->data[M12] = e[m->M12];
    this->data[M13] = e[m->M13];
    this->data[M14] = (T) 0.0;

    this->data[M21] = e[m->M21];
    this->data[M22] = e[m->M22];
    this->data[M23] = e[m->M23];
    this->data[M24] = (T) 0.0;

    this->data[M31] = e[m->M31];
    this->data[M32] = e[m->M32];
    this->data[M33] = e[m->M33];
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;
}

template<typename T>
mat4<T>::mat4(mat4<T> *m)
{
    auto e = m->raw_data();
    this->data = new T[DIM];

    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];
    this->data[M14] = e[M14];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];
    this->data[M24] = e[M24];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];
    this->data[M34] = e[M34];

    this->data[M41] = e[M41];
    this->data[M42] = e[M42];
    this->data[M43] = e[M43];
    this->data[M44] = e[M44];
}

template<typename T>
mat4<T>::mat4(vec4<T> *c1, vec4<T> *c2, vec4<T> *c3, vec4<T> *c4)
{
    this->data = new T[DIM];

    this->data[M11] = c1->x();
    this->data[M12] = c2->x();
    this->data[M13] = c3->x();
    this->data[M14] = c4->x();

    this->data[M21] = c1->y();
    this->data[M22] = c2->y();
    this->data[M23] = c3->y();
    this->data[M24] = c4->y();

    this->data[M31] = c1->z();
    this->data[M32] = c2->z();
    this->data[M33] = c3->z();
    this->data[M34] = c4->z();

    this->data[M41] = c1->w();
    this->data[M42] = c2->w();
    this->data[M43] = c3->w();
    this->data[M44] = c4->w();
}

template<typename T>
mat4<T>::mat4(vec3<T> *c1, vec3<T> *c2, vec3<T> *c3)
{
    this->data = new T[DIM];

    this->data[M11] = c1->x();
    this->data[M12] = c2->x();
    this->data[M13] = c3->x();
    this->data[M14] = (T) 0.0;

    this->data[M21] = c1->y();
    this->data[M22] = c2->y();
    this->data[M23] = c3->y();
    this->data[M24] = (T) 0.0;

    this->data[M31] = c1->z();
    this->data[M32] = c2->z();
    this->data[M33] = c3->z();
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;
}

template<typename T>
mat4<T>::~mat4()
{
    delete [] this->data;
}

template<typename T>
int mat4<T>::dim()
{
    return DIM;
}

template<typename T>
int mat4<T>::rows()
{
    return ROWS;
}

template<typename T>
int mat4<T>::cols()
{
    return COLS;
}

template<typename T>
mat<T> *mat4<T>::set(T *e)
{
    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];
    this->data[M14] = e[M14];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];
    this->data[M24] = e[M24];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];
    this->data[M34] = e[M34];

    this->data[M41] = e[M41];
    this->data[M42] = e[M42];
    this->data[M43] = e[M43];
    this->data[M44] = e[M44];

    return this;
}

template<typename T>
mat<T> *mat4<T>::set(T v)
{
    this->data[M11] = v;
    this->data[M12] = v;
    this->data[M13] = v;
    this->data[M14] = v;

    this->data[M21] = v;
    this->data[M22] = v;
    this->data[M23] = v;
    this->data[M24] = v;

    this->data[M31] = v;
    this->data[M32] = v;
    this->data[M33] = v;
    this->data[M34] = v;

    this->data[M41] = v;
    this->data[M42] = v;
    this->data[M43] = v;
    this->data[M44] = v;

    return this;
}

template<typename T>
mat<T> *mat4<T>::clone()
{
    return new mat4<T>(this);
}

template<typename T>
mat<T> *mat4<T>::copy(mat<T> *m)
{
    if (!dynamic_cast<mat4<T> *>(m))
        return nullptr;

    auto e = m->raw_data();

    this->data[M11] = e[M11];
    this->data[M12] = e[M12];
    this->data[M13] = e[M13];
    this->data[M14] = e[M14];

    this->data[M21] = e[M21];
    this->data[M22] = e[M22];
    this->data[M23] = e[M23];
    this->data[M24] = e[M24];

    this->data[M31] = e[M31];
    this->data[M32] = e[M32];
    this->data[M33] = e[M33];
    this->data[M34] = e[M34];

    this->data[M41] = e[M41];
    this->data[M42] = e[M42];
    this->data[M43] = e[M43];
    this->data[M44] = e[M44];

    return this;
}

template<typename T>
mat<T> *mat4<T>::add(mat<T> *m)
{
    if (!dynamic_cast<mat4<T> *>(m))
        return nullptr;

    auto e = m->raw_data();

    this->data[M11] += e[M11];
    this->data[M12] += e[M12];
    this->data[M13] += e[M13];
    this->data[M14] += e[M14];

    this->data[M21] += e[M21];
    this->data[M22] += e[M22];
    this->data[M23] += e[M23];
    this->data[M24] += e[M24];

    this->data[M31] += e[M31];
    this->data[M32] += e[M32];
    this->data[M33] += e[M33];
    this->data[M34] += e[M34];

    this->data[M41] += e[M41];
    this->data[M42] += e[M42];
    this->data[M43] += e[M43];
    this->data[M44] += e[M44];

    return this;
}

template<typename T>
mat<T> *mat4<T>::sub(mat<T> *m)
{
    if (!dynamic_cast<mat4<T> *>(m))
        return nullptr;

    auto e = m->raw_data();

    this->data[M11] -= e[M11];
    this->data[M12] -= e[M12];
    this->data[M13] -= e[M13];
    this->data[M14] -= e[M14];

    this->data[M21] -= e[M21];
    this->data[M22] -= e[M22];
    this->data[M23] -= e[M23];
    this->data[M24] -= e[M24];

    this->data[M31] -= e[M31];
    this->data[M32] -= e[M32];
    this->data[M33] -= e[M33];
    this->data[M34] -= e[M34];

    this->data[M41] -= e[M41];
    this->data[M42] -= e[M42];
    this->data[M43] -= e[M43];
    this->data[M44] -= e[M44];

    return this;
}

template<typename T>
mat<T> *mat4<T>::s_mul(double s)
{
    this->data[M11] *= s;
    this->data[M12] *= s;
    this->data[M13] *= s;
    this->data[M14] *= s;

    this->data[M21] *= s;
    this->data[M22] *= s;
    this->data[M23] *= s;
    this->data[M24] *= s;

    this->data[M31] *= s;
    this->data[M32] *= s;
    this->data[M33] *= s;
    this->data[M34] *= s;

    this->data[M41] *= s;
    this->data[M42] *= s;
    this->data[M43] *= s;
    this->data[M44] *= s;

    return this;
}

template<typename T>
mat<T> *mat4<T>::l_mul(mat<T> *m)
{
    if (!dynamic_cast<mat4<T> *>(m))
        return nullptr;

    auto e = this->data;
    auto o = m->raw_data();

    T t[this->DIM];

    t[M11] = e[M11] * o[M11] + e[M12] * o[M21] + e[M13] * o[M31] + e[M14] * o[M41];
    t[M12] = e[M11] * o[M12] + e[M12] * o[M22] + e[M13] * o[M32] + e[M14] * o[M42];
    t[M13] = e[M11] * o[M13] + e[M12] * o[M23] + e[M13] * o[M33] + e[M14] * o[M43];
    t[M14] = e[M11] * o[M14] + e[M12] * o[M24] + e[M13] * o[M34] + e[M14] * o[M44];

    t[M21] = e[M21] * o[M11] + e[M22] * o[M21] + e[M23] * o[M31] + e[M24] * o[M41];
    t[M22] = e[M21] * o[M12] + e[M22] * o[M22] + e[M23] * o[M32] + e[M24] * o[M42];
    t[M23] = e[M21] * o[M13] + e[M22] * o[M23] + e[M23] * o[M33] + e[M24] * o[M43];
    t[M24] = e[M21] * o[M14] + e[M22] * o[M24] + e[M23] * o[M34] + e[M24] * o[M44];

    t[M31] = e[M31] * o[M11] + e[M32] * o[M21] + e[M33] * o[M31] + e[M34] * o[M41];
    t[M32] = e[M31] * o[M12] + e[M32] * o[M22] + e[M33] * o[M32] + e[M34] * o[M42];
    t[M33] = e[M31] * o[M13] + e[M32] * o[M23] + e[M33] * o[M33] + e[M34] * o[M43];
    t[M34] = e[M31] * o[M14] + e[M32] * o[M24] + e[M33] * o[M34] + e[M34] * o[M44];

    t[M41] = e[M41] * o[M11] + e[M42] * o[M21] + e[M43] * o[M31] + e[M44] * o[M41];
    t[M42] = e[M41] * o[M12] + e[M42] * o[M22] + e[M43] * o[M32] + e[M44] * o[M42];
    t[M43] = e[M41] * o[M13] + e[M42] * o[M23] + e[M43] * o[M33] + e[M44] * o[M43];
    t[M44] = e[M41] * o[M14] + e[M42] * o[M24] + e[M43] * o[M34] + e[M44] * o[M44];

    e[M11] = t[M11];
    e[M12] = t[M12];
    e[M13] = t[M13];
    e[M14] = t[M14];

    e[M21] = t[M21];
    e[M22] = t[M22];
    e[M23] = t[M23];
    e[M24] = t[M24];

    e[M31] = t[M31];
    e[M32] = t[M32];
    e[M33] = t[M33];
    e[M34] = t[M34];

    e[M41] = t[M41];
    e[M42] = t[M42];
    e[M43] = t[M43];
    e[M44] = t[M44];

    return this;
}

template<typename T>
mat<T> *mat4<T>::r_mul(mat<T> *m)
{
    if (!dynamic_cast<mat4<T> *>(m))
        return nullptr;

    auto e = this->data;
    auto o = m->raw_data();

    T t[this->DIM];

    t[M11] = o[M11] * e[M11] + o[M12] * e[M21] + o[M13] * e[M31] + o[M14] * e[M41];
    t[M12] = o[M11] * e[M12] + o[M12] * e[M22] + o[M13] * e[M32] + o[M14] * e[M42];
    t[M13] = o[M11] * e[M13] + o[M12] * e[M23] + o[M13] * e[M33] + o[M14] * e[M43];
    t[M14] = o[M11] * e[M14] + o[M12] * e[M24] + o[M13] * e[M34] + o[M14] * e[M44];

    t[M21] = o[M21] * e[M11] + o[M22] * e[M21] + o[M23] * e[M31] + o[M24] * e[M41];
    t[M22] = o[M21] * e[M12] + o[M22] * e[M22] + o[M23] * e[M32] + o[M24] * e[M42];
    t[M23] = o[M21] * e[M13] + o[M22] * e[M23] + o[M23] * e[M33] + o[M24] * e[M43];
    t[M24] = o[M21] * e[M14] + o[M22] * e[M24] + o[M23] * e[M34] + o[M24] * e[M44];

    t[M31] = o[M31] * e[M11] + o[M32] * e[M21] + o[M33] * e[M31] + o[M34] * e[M41];
    t[M32] = o[M31] * e[M12] + o[M32] * e[M22] + o[M33] * e[M32] + o[M34] * e[M42];
    t[M33] = o[M31] * e[M13] + o[M32] * e[M23] + o[M33] * e[M33] + o[M34] * e[M43];
    t[M34] = o[M31] * e[M14] + o[M32] * e[M24] + o[M33] * e[M34] + o[M34] * e[M44];

    t[M41] = o[M41] * e[M11] + o[M42] * e[M21] + o[M43] * e[M31] + o[M44] * e[M41];
    t[M42] = o[M41] * e[M12] + o[M42] * e[M22] + o[M43] * e[M32] + o[M44] * e[M42];
    t[M43] = o[M41] * e[M13] + o[M42] * e[M23] + o[M43] * e[M33] + o[M44] * e[M43];
    t[M44] = o[M41] * e[M14] + o[M42] * e[M24] + o[M43] * e[M34] + o[M44] * e[M44];

    e[M11] = t[M11];
    e[M12] = t[M12];
    e[M13] = t[M13];
    e[M14] = t[M14];

    e[M21] = t[M21];
    e[M22] = t[M22];
    e[M23] = t[M23];
    e[M24] = t[M24];

    e[M31] = t[M31];
    e[M32] = t[M32];
    e[M33] = t[M33];
    e[M34] = t[M34];

    e[M41] = t[M41];
    e[M42] = t[M42];
    e[M43] = t[M43];
    e[M44] = t[M44];

    return this;
}

template<typename T>
vec<T> *mat4<T>::v_mul(vec<T> *v)
{
    auto _v = dynamic_cast<vec4<T> *>(v);
    if (!_v)
        return nullptr;

    auto e = this->data;

    return new vec4<T>(
            e[M11] * _v->x() + e[M12] * _v->y() + e[M13] * _v->z() + e[M14] * _v->w(),
            e[M21] * _v->x() + e[M22] * _v->y() + e[M23] * _v->z() + e[M24] * _v->w(),
            e[M31] * _v->x() + e[M32] * _v->y() + e[M33] * _v->z() + e[M34] * _v->w(),
            e[M41] * _v->x() + e[M42] * _v->y() + e[M43] * _v->z() + e[M44] * _v->w()
    );
}

template<typename T>
mat<T> *mat4<T>::identity()
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat4<T> *mat4<T>::scale(vec4<T> *v)
{
    auto tmp = mat4<T>().set_scale(v);

    return dynamic_cast<mat4<T> *>(this->l_mul(tmp));
}

template<typename T>
mat<T> *mat4<T>::scale(T x, T y, T z)
{
    auto tmp = mat4<T>().set_scale(x, y, z);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat4<T>::scale(vec3<T> *v)
{
    auto tmp = mat4<T>().set_scale(v);

    return this->l_mul(tmp);
}

template<typename T>
mat4<T> *mat4<T>::set_scale(vec4<T> *v)
{
    this->data[M11] = v->x();
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = v->y();
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = v->z();
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat4<T>::set_scale(T x, T y, T z)
{
    this->data[M11] = x;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = y;
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = z;
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat4<T>::set_scale(vec3<T> *v)
{
    this->data[M11] = v->x();
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = v->y();
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = v->z();
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat4<T>::rot_x(double deg)
{
    return this->rot_x_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat4<T>::rot_x_r(double rad)
{
    auto tmp = mat4<T>().set_rot_x_r(rad);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat4<T>::rot_y(double deg)
{
    return this->rot_y_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat4<T>::rot_y_r(double rad)
{
    auto tmp = mat4<T>().set_rot_y_r(rad);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat4<T>::rot_z(double deg)
{
    return this->rot_z_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat4<T>::rot_z_r(double rad)
{
    auto tmp = mat4<T>().set_rot_z_r(rad);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat4<T>::set_rot_x(double deg)
{
    return this->set_rot_x_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat4<T>::set_rot_x_r(double rad)
{
    auto s = trig_round_err(std::sin(rad));
    auto c = trig_round_err(std::cos(rad));

    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) c;
    this->data[M23] = (T) -1.0 * s;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) s;
    this->data[M33] = (T) c;
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat4<T>::set_rot_y(double deg)
{
    return this->set_rot_y_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat4<T>::set_rot_y_r(double rad)
{
    auto s = trig_round_err(std::sin(rad));
    auto c = trig_round_err(std::cos(rad));

    this->data[M11] = (T) c;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) s;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) -1.0 * s;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) c;
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat4<T>::set_rot_z(double deg)
{
    return this->set_rot_y_r(DEG_TO_RADS(deg));
}

template<typename T>
mat<T> *mat4<T>::set_rot_z_r(double rad)
{
    auto s = trig_round_err(std::sin(rad));
    auto c = trig_round_err(std::cos(rad));

    this->data[M11] = (T) c;
    this->data[M12] = (T) -1.0 * s;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) s;
    this->data[M22] = (T) c;
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;
    this->data[M34] = (T) 0.0;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat4<T> *mat4<T>::translate(T x, T y, T z, T w)
{
    auto tmp = mat4<T>().set_translate(x, y, z, w);

    return dynamic_cast<mat4<T> *>(this->l_mul(tmp));
}

template<typename T>
mat4<T> *mat4<T>::translate(vec4<T> *v)
{
    auto tmp = mat4<T>().set_translate(v);

    return dynamic_cast<mat4<T> *>(this->l_mul(tmp));
}

template<typename T>
mat<T> *mat4<T>::translate(T x, T y, T z)
{
    auto tmp = mat4<T>().set_translate(x, y, z);

    return this->l_mul(tmp);
}

template<typename T>
mat<T> *mat4<T>::translate(vec3<T> *v)
{
    auto tmp = mat4<T>().set_translate(v);

    return this->l_mul(tmp);
}

template<typename T>
mat4<T> *mat4<T>::set_translate(T x, T y, T z, T w)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = x;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;
    this->data[M24] = y;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;
    this->data[M34] = z;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = w;

    return this;
}

template<typename T>
mat4<T> *mat4<T>::set_translate(vec4<T> *v)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = v->x();

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;
    this->data[M24] = v->y();

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;
    this->data[M34] = v->z();

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = v->w();

    return this;
}

template<typename T>
mat<T> *mat4<T>::set_translate(T x, T y, T z)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = x;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;
    this->data[M24] = y;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;
    this->data[M34] = z;

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat<T> *mat4<T>::set_translate(vec3<T> *v)
{
    this->data[M11] = (T) 1.0;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = v->x();

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 1.0;
    this->data[M23] = (T) 0.0;
    this->data[M24] = v->y();

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) 1.0;
    this->data[M34] = v->z();

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
mat4<T> *mat4<T>::perspective(double fov_y, double aspect, double near, double far)
{
    fov_y = DEG_TO_RADS(fov_y) / 2;

    double t = near * trig_round_err(std::tan(fov_y));
    double r = t * aspect;

    this->data[M11] = (T) near / r;
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) 0.0;

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) near / t;
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) 0.0;

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) -1.0 * (far + near) / (far - near);
    this->data[M34] = (T) (-2.0 * near * far) / (far - near);

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) -1.0;
    this->data[M44] = (T) 0.0;

    return this;
}

template<typename T>
mat4<T> *mat4<T>::orthographic(double left,
                              double right,
                              double top,
                              double bottom,
                              double near,
                              double far)
{
    this->data[M11] = (T) 2.0 / (right - left);
    this->data[M12] = (T) 0.0;
    this->data[M13] = (T) 0.0;
    this->data[M14] = (T) -(right + left) / (right - left);

    this->data[M21] = (T) 0.0;
    this->data[M22] = (T) 2.0 / (top - bottom);
    this->data[M23] = (T) 0.0;
    this->data[M24] = (T) -(top + bottom) / (top - bottom);

    this->data[M31] = (T) 0.0;
    this->data[M32] = (T) 0.0;
    this->data[M33] = (T) -2.0 / (far - near);
    this->data[M34] = (T) -(far + near) / (far - near);

    this->data[M41] = (T) 0.0;
    this->data[M42] = (T) 0.0;
    this->data[M43] = (T) 0.0;
    this->data[M44] = (T) 1.0;

    return this;
}

template<typename T>
double mat4<T>::det()
{
    auto e = this->data;

    return ((e[M11] * (e[M22] * (e[M33] * e[M44] - e[M34] * e[M43]) +
             e[M23] * (e[M34] * e[M42] - e[M32] * e[M44]) +
             e[M24] * (e[M32] * e[M43] - e[M33] * e[M42]))) -
            (e[M12] * (e[M21] * (e[M33] * e[M44] - e[M34] * e[M43]) +
             e[M23] * (e[M34] * e[M41] - e[M31] * e[M44]) +
             e[M24] * (e[M31] * e[M43] - e[M33] * e[M41]))) +
            (e[M13] * (e[M21] * (e[M32] * e[M44] - e[M34] * e[M42]) +
             e[M22] * (e[M34] * e[M41] - e[M31] * e[M44]) +
             e[M24] * (e[M31] * e[M42] - e[M32] * e[M41]))) -
            (e[M14] * (e[M21] * (e[M32] * e[M43] - e[M33] * e[M42]) +
             e[M22] * (e[M33] * e[M41] - e[M31] * e[M43]) +
             e[M23] * (e[M31] * e[M42] - e[M32] * e[M41]))));
}

template<typename T>
mat<T> *mat4<T>::transpose()
{
    auto tmp = this->data[M12];
    this->data[M12] = this->data[M21];
    this->data[M21] = tmp;

    tmp = this->data[M13];
    this->data[M13] = this->data[M31];
    this->data[M31] = tmp;

    tmp = this->data[M14];
    this->data[M14] = this->data[M41];
    this->data[M41] = tmp;

    tmp = this->data[M23];
    this->data[M23] = this->data[M32];
    this->data[M32] = tmp;

    tmp = this->data[M24];
    this->data[M24] = this->data[M42];
    this->data[M42] = tmp;

    tmp = this->data[M34];
    this->data[M34] = this->data[M43];
    this->data[M43] = tmp;

    return this;
}

template<typename T>
mat<T> *mat4<T>::inverse()
{
    double det = this->det();
    if (det == 0)
        throw div_by_zero_ex("Can't invert a singular mat4");

    double inv_det = 1.0 / det;
    T tmp[DIM];
    auto e = this->data;

    tmp[M11] = e[M23] * e[M34] * e[M42] - e[M24] * e[M33] * e[M42] +
               e[M24] * e[M32] * e[M43] - e[M22] * e[M34] * e[M43] -
               e[M23] * e[M32] * e[M44] + e[M22] * e[M33] * e[M44];
    tmp[M12] = e[M24] * e[M33] * e[M41] - e[M23] * e[M34] * e[M41] -
               e[M24] * e[M31] * e[M43] + e[M21] * e[M34] * e[M43] +
               e[M23] * e[M31] * e[M44] - e[M21] * e[M33] * e[M44];
    tmp[M13] = e[M22] * e[M34] * e[M41] - e[M24] * e[M32] * e[M41] +
               e[M24] * e[M31] * e[M42] - e[M21] * e[M34] * e[M42] -
               e[M22] * e[M31] * e[M44] + e[M21] * e[M32] * e[M44];
    tmp[M14] = e[M23] * e[M32] * e[M41] - e[M22] * e[M33] * e[M41] -
               e[M23] * e[M31] * e[M42] + e[M21] * e[M33] * e[M42] +
               e[M22] * e[M31] * e[M43] - e[M21] * e[M32] * e[M43];

    tmp[M21] = e[M14] * e[M33] * e[M42] - e[M13] * e[M34] * e[M43] -
               e[M14] * e[M32] * e[M43] + e[M12] * e[M34] * e[M43] +
               e[M13] * e[M32] * e[M44] - e[M12] * e[M33] * e[M44];
    tmp[M22] = e[M13] * e[M34] * e[M41] - e[M14] * e[M33] * e[M41] +
               e[M14] * e[M31] * e[M43] - e[M11] * e[M34] * e[M43] -
               e[M13] * e[M31] * e[M44] + e[M11] * e[M33] * e[M44];
    tmp[M23] = e[M14] * e[M32] * e[M41] - e[M12] * e[M34] * e[M41] -
               e[M14] * e[M31] * e[M42] + e[M11] * e[M34] * e[M42] +
               e[M12] * e[M31] * e[M44] - e[M11] * e[M32] * e[M44];
    tmp[M24] = e[M12] * e[M33] * e[M41] - e[M13] * e[M32] * e[M41] +
               e[M13] * e[M31] * e[M42] - e[M11] * e[M33] * e[M42] -
               e[M12] * e[M31] * e[M43] + e[M11] * e[M32] * e[M43];

    tmp[M31] = e[M13] * e[M24] * e[M43] - e[M14] * e[M23] * e[M42] +
               e[M14] * e[M22] * e[M43] - e[M12] * e[M24] * e[M43] -
               e[M13] * e[M22] * e[M44] + e[M12] * e[M23] * e[M44];
    tmp[M32] = e[M14] * e[M23] * e[M41] - e[M13] * e[M24] * e[M41] -
               e[M14] * e[M21] * e[M43] + e[M11] * e[M24] * e[M43] +
               e[M13] * e[M21] * e[M44] - e[M11] * e[M23] * e[M44];
    tmp[M33] = e[M12] * e[M24] * e[M41] - e[M14] * e[M22] * e[M41] +
               e[M14] * e[M21] * e[M42] - e[M11] * e[M24] * e[M42] -
               e[M12] * e[M21] * e[M44] + e[M11] * e[M22] * e[M44];
    tmp[M34] = e[M13] * e[M22] * e[M41] - e[M12] * e[M23] * e[M41] -
               e[M13] * e[M21] * e[M42] + e[M11] * e[M23] * e[M42] +
               e[M12] * e[M21] * e[M43] - e[M11] * e[M22] * e[M43];

    tmp[M41] = e[M14] * e[M23] * e[M32] - e[M13] * e[M24] * e[M32] -
               e[M14] * e[M22] * e[M33] + e[M12] * e[M24] * e[M33] +
               e[M13] * e[M22] * e[M34] - e[M12] * e[M23] * e[M34];
    tmp[M42] = e[M13] * e[M24] * e[M31] - e[M14] * e[M23] * e[M31] +
               e[M14] * e[M21] * e[M33] - e[M11] * e[M24] * e[M33] -
               e[M13] * e[M21] * e[M34] + e[M11] * e[M23] * e[M34];
    tmp[M43] = e[M14] * e[M22] * e[M31] - e[M12] * e[M24] * e[M31] -
               e[M14] * e[M21] * e[M32] + e[M11] * e[M24] * e[M32] +
               e[M12] * e[M21] * e[M34] - e[M11] * e[M22] * e[M34];
    tmp[M44] = e[M12] * e[M23] * e[M31] - e[M13] * e[M22] * e[M31] +
               e[M13] * e[M21] * e[M32] - e[M11] * e[M23] * e[M32] -
               e[M12] * e[M21] * e[M33] + e[M11] * e[M22] * e[M33];

    e[M11] = inv_det * tmp[M11];
    e[M12] = inv_det * tmp[M12];
    e[M13] = inv_det * tmp[M13];
    e[M14] = inv_det * tmp[M14];

    e[M21] = inv_det * tmp[M21];
    e[M22] = inv_det * tmp[M22];
    e[M23] = inv_det * tmp[M23];
    e[M24] = inv_det * tmp[M24];

    e[M31] = inv_det * tmp[M31];
    e[M32] = inv_det * tmp[M32];
    e[M33] = inv_det * tmp[M33];
    e[M34] = inv_det * tmp[M34];

    e[M41] = inv_det * tmp[M41];
    e[M42] = inv_det * tmp[M42];
    e[M43] = inv_det * tmp[M43];
    e[M44] = inv_det * tmp[M44];

    return this;
}

template<typename T>
T *mat4<T>::raw_data()
{
    return this->data;
}

template<typename T>
T &mat4<T>::operator[](int i)
{
    if (i < 0 || i >= DIM)
        throw out_of_bounds_ex(
                "Index: " + std::to_string(i) + " out of bounds for mat4");

    return this->data[i];
}

template<typename T>
mat4<T> *mat4<T>::operator+(mat4<T> *m)
{
    return dynamic_cast<mat4<T> *>(this->add(m));
}

template<typename T>
void mat4<T>::operator+=(mat4<T> *m)
{
    this->add(m);
}

template<typename T>
mat4<T> *mat4<T>::operator-(mat4<T> *m)
{
    return dynamic_cast<mat4<T> *>(this->sub(m));
}

template<typename T>
void mat4<T>::operator-=(mat4<T> *m)
{
    this->sub(m);
}

template<typename T>
mat4<T> *mat4<T>::operator*(double s)
{
    return dynamic_cast<mat4<T> *>(this->s_mul(s));
}

template<typename T>
void mat4<T>::operator*=(double s)
{
    this->s_mul(s);
}

template<typename T>
mat4<T> *mat4<T>::operator*(mat4<T> *m)
{
    return dynamic_cast<mat4<T> *>(this->r_mul(m));
}

template<typename T>
bool mat4<T>::operator==(mat4<T> *m) const
{
    auto e = m->raw_data();

    for (auto i = 0; i < DIM; i++) {
        if (this->data[i] != e[i])
            return false;
    }

    return true;
}

template<typename T>
bool mat4<T>::operator!=(mat4<T> *m) const
{
    return !(*this == m);
}

template<typename T>
mat4<T> &mat4<T>::operator=(mat4<T> *m)
{
    (void) this->copy(m);

    return *this;
}

template<typename T>
mat4<T> *mat4<T>::operator!()
{
    return dynamic_cast<mat4<T> *>(this->inverse());
}

template<typename T>
double mat4<T>::operator~()
{
    return this->det();
}

template<typename T>
mat4<T> *mat4<T>::operator-()
{
    return dynamic_cast<mat4<T> *>(this->transpose());
}

template<typename T>
std::string mat4<T>::str()
{
    return "[" + std::to_string(this->data[M11]) + ", " +
            std::to_string(this->data[M12]) + ", " +
            std::to_string(this->data[M13]) + ", " +
            std::to_string(this->data[M14]) + "]\n" +
            "[" + std::to_string(this->data[M21]) + ", " +
            std::to_string(this->data[M22]) + ", " +
            std::to_string(this->data[M23]) + ", " +
            std::to_string(this->data[M24]) + "]\n" +
            "[" + std::to_string(this->data[M31]) + ", " +
            std::to_string(this->data[M32]) + ", " +
            std::to_string(this->data[M33]) + ", " +
            std::to_string(this->data[M34]) + "]\n" +
            "[" + std::to_string(this->data[M41]) + ", " +
            std::to_string(this->data[M42]) + ", " +
            std::to_string(this->data[M43]) + ", " +
            std::to_string(this->data[M44]) + "]\n";
}

}

#endif //GLW_MAT4_HPP
