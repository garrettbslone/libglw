//
// Created by Garrett on 9/13/21.
//

#ifndef LIBGLW_VEC_HPP
#define LIBGLW_VEC_HPP

#include <string>

namespace glw {

/*
 * Base abstract class for all R^n vectors. For all functions that take a
 * vec<T> * as an argument, the argument must be of the same dimension of
 * this vec.
 */
template<typename T>
class vec {
public:
    virtual ~vec() = 0;

    /*
     * Dimension of the vector to be used for sanity checks on overloaded
     * [] operator.
     */
    virtual int dim() = 0;

    /*
     * Return a copy of this vec. Reference must be freed by the caller.
     */
    virtual vec<T> *clone() = 0;
    /*
     * Copy the vec v to this vec and return this vec for chaining.
     */
    virtual vec<T> *copy(vec<T> *v) = 0;

    /*
     * Add v to this vec and return this vec for chaining.
     */
    virtual vec<T> *add(vec<T> *v) = 0;
    /*
     * Subtract v from this vec and return this vec for chaining.
     */
    virtual vec<T> *sub(vec<T> *v) = 0;
    /*
     * Scale this vec by s and return this vec for chaining.
     */
    virtual vec<T> *scale(double s) = 0;
    /*
     * Negate this vec and return this vec for chaining.
     */
    virtual vec<T> *negate() = 0;

    /*
     * Return the dot product between v and this vec.
     */
    virtual double dot(vec<T> *v) = 0;
    /*
     * Return the magnitude (length) of this vec.
     */
    virtual double mag() = 0;
    /*
     * Return the square of the magnitude of this vec. Is faster than using
     * mag(), but not for accurate magnitudes.
     */
    virtual double mag2() = 0;

    /*
     * Project this vec onto the onto vec and return this vec for chaining.
     */
    virtual vec<T> *project(vec<T> *onto) = 0;
    /*
     * Normalize this vec turning it into a unit vector (divide by its
     * magnitude) and return this vec for chaining.
     */
    virtual vec<T> *normalize() = 0;
    /*
     * Linearly interpolates between this vec and the target vec by alpha
     * which is in the range [0,1] and store the resulting vec in this vec.
     * Return this vector for chaining.
     */
    virtual vec<T> *lerp(vec<T> *target, double a) = 0;

    /*
     * Return the distance between the to vec and this vec.
     */
    virtual double dist(vec<T> *to) = 0;
    /*
     * Return the square of the distance between the to vec and this vec.
     */
    virtual double dist2(vec<T> *to) = 0;

    /*
     * Return whether the vector has a magnitude of 1.
     */
    virtual bool is_unit() = 0;
    /*
     * Return whether the vector is a zero vector.
     */
    virtual bool is_zero() = 0;
    /*
     * Return whether the vec v is on the same line as this vec.
     */
    virtual bool is_collinear(vec<T> *v) = 0;
    /*
     * Return whether the vec v is perpendicular to this vec.
     */
    virtual bool is_perpendicular(vec<T> *v) = 0;
    /*
     * Return whether the vec v is in the same direction as this vec.
     */
    virtual bool is_same_dir(vec<T> *v) = 0;

    /*
     * Return the components of this vec as an array;
     */
    virtual T *raw_data() = 0;

    /*
     * Return the string representation of this vec. This is and alternative
     * to overloading the ostream operator which doesn't handle templates well.
     */
    virtual std::string str() = 0;

protected:
    T *data;
};

template<typename T>
vec<T>::~vec()
= default;

}

#endif //LIBGLW_VEC_HPP
