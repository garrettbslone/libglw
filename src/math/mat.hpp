//
// Created by Garrett on 9/24/21.
//

#ifndef GLW_MAT_HPP
#define GLW_MAT_HPP

#include "vec3.hpp"
#include "vec4.hpp"

namespace glw {

template<typename T>
class mat {
public:
    virtual ~mat() = 0;

    virtual int dim() = 0;
    virtual int rows() = 0;
    virtual int cols() = 0;

    /*
     * Set the elements of this mat to the elements of a singular array e.
     * e must be at least as large as this->dim();
     */
    virtual mat<T> *set(T *e) = 0;
    /*
     * Set all elements of this mat to v;
     */
    virtual mat<T> *set(T v) = 0;
    /*
     * Return a copy of this mat. Reference must be freed by the caller.
     */
    virtual mat<T> *clone() = 0;
    /*
     * Copy the elements of m to this mat.
     */
    virtual mat<T> *copy(mat<T> *m) = 0;

    virtual mat<T> *add(mat<T> *m) = 0;
    virtual mat<T> *sub(mat<T> *m) = 0;

    /*
     * Multiply this mat by the scalar s.
     */
    virtual mat<T> *s_mul(double s) = 0;
    /*
     * Multiply this mat with m where this is the left side mat: this * m.
     */
    virtual mat<T> *l_mul(mat<T> *m) = 0;
    /*
     * Multiply this mat with m where this is the right side mat: m * this.
     */
    virtual mat<T> *r_mul(mat<T> *m) = 0;
    /*
     * Multiply this mat with a column vec where v->dim() == this->cols().
     */
    virtual vec<T> *v_mul(vec<T> *v) = 0;

    virtual mat<T> *identity() = 0;

    /*
     * Apply the scale of the arg(s) to this mat. Equivalent to:
     * auto s = new mat<T>().set_scale();
     * this.l_mul(s);
     */
    virtual mat<T> *scale(T x, T y, T z) = 0;
    virtual mat<T> *scale(vec3<T> *v) = 0;

    /*
     * Set this mat to the scale passed in the arg(s).
     */
    virtual mat<T> *set_scale(T x, T y, T z) = 0;
    virtual mat<T> *set_scale(vec3<T> *v) = 0;

    /*
     * Apply a rotation to this mat around the corresponding axis
     * in the function name. Functions with a trailing '_r' take
     * the angle in radians while the others take it in degrees.
     * Equivalent to:
     * auto r = new mat<T>().set_rot();
     * this.l_mul(r);
     */
    virtual mat<T> *rot_x(double deg) = 0;
    virtual mat<T> *rot_x_r(double rad) = 0;
    virtual mat<T> *rot_y(double deg) = 0;
    virtual mat<T> *rot_y_r(double rad) = 0;
    virtual mat<T> *rot_z(double deg) = 0;
    virtual mat<T> *rot_z_r(double rad) = 0;

    /*
     * Set this mat to a rotation of the angle passed in.
     */
    virtual mat<T> *set_rot_x(double deg) = 0;
    virtual mat<T> *set_rot_x_r(double rad) = 0;
    virtual mat<T> *set_rot_y(double deg) = 0;
    virtual mat<T> *set_rot_y_r(double rad) = 0;
    virtual mat<T> *set_rot_z(double deg) = 0;
    virtual mat<T> *set_rot_z_r(double rad) = 0;

    /*
     * Translate this mat by the arg(s) passed in. Equivalent to:
     * auto t = new mat<T>().set_translation();
     * this.l_mul(t);
     */
    virtual mat<T> *translate(T x, T y, T z) = 0;
    virtual mat<T> *translate(vec3<T> *v) = 0;

    /*
     * Set this mat to the translation described by the arg(s) passed in.
     */
    virtual mat<T> *set_translate(T x, T y, T z) = 0;
    virtual mat<T> *set_translate(vec3<T> *v) = 0;

    /*
     * Returns the determinant of this mat.
     */
    virtual double det() = 0;
    virtual mat<T> *transpose() = 0;
    /*
     * Attempts to inverse this mat. Throws a glw::div_by_zero_ex if trying to
     * invert a singular mat.
     */
    virtual mat<T> *inverse() = 0;

    /*
     * Return a pointer to the raw array data of this mat.
     */
    virtual T *raw_data() = 0;

    /*
     * Returns a string representation of this mat of the form:
     * [M11, M12, ..., M1M]
     * [M21, M22, ..., M2M]
     * [..., ..., ..., ...]
     * [MN1, MN2, ..., MNM]
     */
    virtual std::string str() = 0;

protected:
    T *data;
};

template<typename T>
mat<T>::~mat()
= default;

}

#endif //GLW_MAT_HPP
