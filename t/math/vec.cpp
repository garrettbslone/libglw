//
// Created by Garrett on 9/18/21.
//

#include <iostream>
#include <glw.hpp>

#include "../include/test_res.hpp"

using namespace glw;

using std::cout;
using std::endl;
using std::string;
using std::to_string;

void vec_base_test(test_res *res, int dim, vec<double> *u);
void vec_arithmetic_test(test_res *res, vec<double> *u, vec<double> *v, vec<double> *w);
void vec2_operator_test(test_res *res);
void vec3_operator_test(test_res *res);
void vec4_operator_test(test_res *res);
void vec_scalar_test(test_res *res, vec<double> *u, vec<double> *v);
void vec_vec_test(test_res *res, vec<double> *u, vec<double> *v, vec<double> *w);
void vec_bool_test(test_res *res, vec<double> *u, vec<double> *v, vec<double> *w);

void vec_raw_eq(test_res *res, const double *u, const double *v, int dim, const string &func);

int main(int argc, char *argv[])
{
    auto *res = new test_res();

    vec2<double> *v2[3] = {
            new vec2<double>(1.0, 1.0),
            new vec2<double>(-1.0, -3.0),
            new vec2<double>(0.0, -2.0)
    };

    vec3<double> *v3[3] = {
            new vec3<double>(1.0, 1.0, 1.0),
            new vec3<double>(1.0, 2.0, 3.0),
            new vec3<double>(2.0, 3.0, 4.0)
    };

    vec4<double> *v4[3] = {
            new vec4<double>(1.0, 1.0, 1.0, 1.0),
            new vec4<double>(-4.0, 4.0, -6.0, 5.0),
            new vec4<double>(-3.0, 5.0, -5.0, 6.0)
    };

    cout << "starting...\n";
    cout << "basic...\n";

    vec_base_test(res, 2, v2[0]);
    vec_base_test(res, 3, v3[0]);
    vec_base_test(res, 4, v4[0]);

    (void) v2[0]->set(1.0, 1.0);
    (void) v3[0]->set(1.0, 1.0, 1.0);
    (void) v4[0]->set(1.0, 1.0, 1.0, 1.0);

    cout << "arithmetic...\n";

    vec_arithmetic_test(res, v2[0]->clone(), v2[1]->clone(), v2[2]->clone());
    vec_arithmetic_test(res, v3[0]->clone(), v3[1]->clone(), v3[2]->clone());
    vec_arithmetic_test(res, v4[0]->clone(), v4[1]->clone(), v4[2]->clone());

    cout << "operators...\n";

    vec2_operator_test(res);
    vec3_operator_test(res);
    vec4_operator_test(res);

    cout << "scalars...\n";

    vec_scalar_test(res, v2[0]->clone(), v2[1]->clone());
    vec_scalar_test(res, v3[0]->clone(), v3[1]->clone());
    vec_scalar_test(res, v4[0]->clone(), v4[1]->clone());

    cout << "vecs...\n";

    (void) v2[2]->set(-2.0, -2.0);
    (void) v3[2]->set(2.0, 2.0, 2.0);
    (void) v4[2]->set(-0.25, -0.25, -0.25, -0.25);

    vec_vec_test(res, v2[0], v2[1], v2[2]);
    vec_vec_test(res, v3[0], v3[1], v3[2]);
    vec_vec_test(res, v4[0], v4[1], v4[2]);

    cout << "bools...\n";

    (void) v2[1]->set(0.0, 0.0);
    (void) v3[1]->set(0.0, 0.0, 0.0);
    (void) v4[1]->set(0.0, 0.0, 0.0, 0.0);

    vec_bool_test(res, &vec2<double>::X, &vec2<double>::ZERO, &vec2<double>::Y);
    vec_bool_test(res, &vec3<double>::X, &vec3<double>::ZERO, &vec3<double>::Y);
    vec_bool_test(res, &vec4<double>::X, &vec4<double>::ZERO, &vec4<double>::Y);

    cout << *res;

    delete res;

    return 0;
}

void vec_base_test(test_res *res, int dim, vec<double> *u)
{
    res->inc_total(5);

    if (dim != u->dim())
        res->fail("vec" + to_string(dim) + ": dim");
    else
        res->pass();

    switch (dim) {
    case 2: {
        double tmp[2] = {1.0, 1.0};
        vec_raw_eq(res, u->raw_data(), tmp, 2, "raw_data");

        break;
    }
    case 3: {
        double tmp[3] = {1.0, 1.0, 1.0};
        vec_raw_eq(res, u->raw_data(), tmp, 3, "raw_data");

        break;
    }
    case 4:
    default: {
        double tmp[4] = {1.0, 1.0, 1.0, 1.0};
        vec_raw_eq(res, u->raw_data(), tmp, 4, "raw_data");

        break;
    }
    }

    auto v = u->clone();

    for (auto i = 0; i < u->dim(); i++) {
        if (u->raw_data()[i] != v->raw_data()[i]) {
            res->fail("vec" + to_string(u->dim()) + ": clone");
            break;
        }

        if (i == u->dim() - 1) {
            v->raw_data()[i] *= 2.0;

            if (u->raw_data()[i] != v->raw_data()[i])
                res->pass();
            else
                res->fail("vec" + to_string(u->dim()) + ": clone");
        }
    }

    v->copy(u);

    vec_raw_eq(res, v->raw_data(), u->raw_data(), v->dim(), "copy");

    switch (dim) {
    case 2: {
        auto w = dynamic_cast<vec2<double> *>(v);
        w->set(1.0, -1.0);

        if (w->raw_data()[0] != 1.0 || w->raw_data()[1] != -1.0)
            res->fail("vec2: set");
        else
            res->pass();

        break;
    }
    case 3: {
        auto w = dynamic_cast<vec3<double> *>(v);
        w->set(1.0, -1.0, 2.0);

        if (w->raw_data()[0] != 1.0 || w->raw_data()[1] != -1.0 ||
            w->raw_data()[2] != 2.0)
            res->fail("vec3: set");
        else
            res->pass();

        break;
    }
    case 4: {
    default:
        auto w = dynamic_cast<vec4<double> *>(v);
        w->set(1.0, -1.0, 2.0, -2.0);

        if (w->raw_data()[0]!=1.0 || w->raw_data()[1]!=-1.0 ||
            w->raw_data()[2]!=2.0 || w->raw_data()[3]!=-2.0)
            res->fail("vec4: set");
        else
            res->pass();

        break;
    }
    }

    delete v;
}

void vec_arithmetic_test(test_res *res, vec<double> *u, vec<double> *v, vec<double> *w)
{
    const int num_tests = 5;

    res->inc_total(num_tests);

    if (u->dim() != v->dim() || u->dim() != w->dim() || v->dim() != w->dim()) {
        res->fail(num_tests, "vectors u, v, & w are not all of the same dimension");

        delete u;
        delete v;
        delete w;
        return;
    }

    auto _u = u->clone();

    u->add(v);

    vec_raw_eq(res, u->raw_data(), w->raw_data(), u->dim(), "add");

    u->sub(v);

    vec_raw_eq(res, u->raw_data(), _u->raw_data(), u->dim(), "sub");

    delete _u;

    double tmp[u->dim()];

    for (auto i = 0; i < u->dim(); i++)
        tmp[i] = u->raw_data()[i] * 2.0;

    u->scale(2.0);

    vec_raw_eq(res, u->raw_data(), tmp, u->dim(), "scale");

    for (auto i = 0; i < u->dim(); i++)
        tmp[i] = u->raw_data()[i] * -1.0;

    u->negate();

    vec_raw_eq(res, u->raw_data(), tmp, u->dim(), "sub");

    double dot = 0.0;

    for (auto i = 0; i < u->dim(); i++)
        dot += u->raw_data()[i] * v->raw_data()[i];

    if (u->dot(v) != dot)
        res->fail("vec" + to_string(u->dim()) + ": dot");
    else
        res->pass();

    delete u;
    delete v;
    delete w;
}

void vec2_operator_test(test_res *res)
{
    res->inc_total(6);

    auto u = new vec2<double>(1.0, 1.0);
    auto v = new vec2<double>(1.0, 1.0);
    auto w = new vec2<double>(2.0, 2.0);

    if (*u == v)
        res->pass();
    else
        res->fail("vec2: operator=");

    if (*u != w)
        res->pass();
    else
        res->fail("vec2: operator!=");

    if (*(*u + v) == w)
        res->pass();
    else
        res->fail("vec2: operator+");

    if (*(*u - v) == v)
        res->pass();
    else
        res->fail("vec2: operator-");

    if (*u * v == 2.0)
        res->pass();
    else
        res->fail("vec2: operator*: dot");

    if (*(*u * 2.0) == w)
        res->pass();
    else
        res->fail("vec2: operator*: scale");

    delete u;
    delete v;
    delete w;
}

void vec3_operator_test(test_res *res)
{
    res->inc_total(8);

    auto u = new vec3<double>(1.0, 1.0, 1.0);
    auto v = new vec3<double>(1.0, 1.0, 1.0);
    auto w = new vec3<double>(2.0, 2.0, 2.0);

    if (*u == v)
        res->pass();
    else
        res->fail("vec3: operator=");

    if (*u != w)
        res->pass();
    else
        res->fail("vec3: operator!=");

    if (*(*u + v) == w)
        res->pass();
    else
        res->fail("vec3: operator+");

    if (*(*u - v) == v)
        res->pass();
    else
        res->fail("vec3: operator-");

    if (*u * v == 3.0)
        res->pass();
    else
        res->fail("vec3: operator*: dot");

    if (*(*u * 2.0) == w)
        res->pass();
    else
        res->fail("vec3: operator*: scale");

    v->set(-1.0, 0.0, 1.0);

    auto c = u->cross(v);
    double tmp[3] = { -2.0, 4.0, -2.0 };

    vec_raw_eq(res, c->raw_data(), tmp, 3, "vec3: cross");

    c->set(0.0, 0.0, 0.0);
    c->copy(*u / v);

    vec_raw_eq(res, c->raw_data(), tmp, 3, "vec3: operator/: cross");

    delete u;
    delete v;
    delete w;
}

void vec4_operator_test(test_res *res)
{
    res->inc_total(6);

    auto u = new vec4<double>(1.0, 1.0, 1.0, 1.0);
    auto v = new vec4<double>(1.0, 1.0, 1.0, 1.0);
    auto w = new vec4<double>(2.0, 2.0, 2.0, 2.0);

    if (*u == v)
        res->pass();
    else
        res->fail("vec4: operator=");

    if (*u != w)
        res->pass();
    else
        res->fail("vec4: operator!=");

    if (*(*u + v) == w)
        res->pass();
    else
        res->fail("vec4: operator+");

    if (*(*u - v) == v)
        res->pass();
    else
        res->fail("vec4: operator-");

    if (*u * v == 4.0)
        res->pass();
    else
        res->fail("vec4: operator*: dot");

    if (*(*u * 2.0) == w)
        res->pass();
    else
        res->fail("vec4: operator*: scale");

    delete u;
    delete v;
    delete w;
}

void vec_scalar_test(test_res *res, vec<double> *u, vec<double> *v)
{
    const int num_tests = 4;

    res->inc_total(num_tests);

    if (u->dim() != v->dim()) {
        res->fail(num_tests, "vectors u & v are not of the same dimension");

        delete u;
        delete v;
        return;
    }

    double tmp = u->dot(u);

    tmp = sqrt(tmp);

    if (u->mag() != tmp)
        res->fail("vec" + to_string(u->dim()) + ": mag");
    else
        res->pass();

    tmp = u->dot(u);

    if (u->mag2() != tmp)
        res->fail("vec" + to_string(u->dim()) + ": mag");
    else
        res->pass();

    tmp = 0.0;
    for (auto i = 0; i < u->dim(); i++) {
        tmp += pow(u->raw_data()[i] - v->raw_data()[i], 2);
    }

    if (u->dist2(v) != tmp)
        res->fail("vec" + to_string(u->dim()) + ": dist2");
    else
        res->pass();

    tmp = sqrt(tmp);

    if (u->dist(v) != tmp)
        res->fail("vec" + to_string(u->dim()) + ": dist");
    else
        res->pass();

    delete u;
    delete v;
}

void vec_vec_test(test_res *res, vec<double> *u, vec<double> *v, vec<double> *w)
{
    const int num_tests = 3;

    res->inc_total(num_tests);

    if (u->dim() != v->dim() || u->dim() != w->dim() || v->dim() != w->dim()) {
        res->fail(num_tests, "vectors u, v, & w are not all of the same dimension");
        return;
    }

    vec_raw_eq(res, v->project(u->clone())->raw_data(), w->raw_data(), v->dim(), "project");

    w->copy(v->scale(1 / v->mag()));
    vec_raw_eq(res, v->raw_data(), w->raw_data(), v->dim(), "normalize");

    for (auto i = 0; i < v->dim(); i++) {
        w->raw_data()[i] = v->raw_data()[i] + (0.3 * (u->raw_data()[i] - v->raw_data()[i])) ;
    }

    vec_raw_eq(res, v->lerp(u, 0.3)->raw_data(), w->raw_data(), v->dim(), "lerp");
}

void vec_bool_test(test_res *res, vec<double> *u, vec<double> *v, vec<double> *w)
{
    const int num_tests = 7;

    res->inc_total(num_tests);

    if (u->dim() != v->dim() || u->dim() != w->dim() || v->dim() != w->dim()) {
        res->fail(num_tests, "vectors u, v, & w are not all of the same dimension");
        return;
    }

    if (!u->is_unit())
        res->fail("vec" + to_string(u->dim()) + ": is_unit");
    else
        res->pass();

    if (v->is_unit())
        res->fail("vec" + to_string(u->dim()) + ": is_unit");
    else
        res->pass();

    if (u->is_zero())
        res->fail("vec" + to_string(u->dim()) + ": is_zero");
    else
        res->pass();

    if (!v->is_zero())
        res->fail("vec" + to_string(u->dim()) + ": is_zero");
    else
        res->pass();

    auto _u = u->clone()->scale(5.0);

    if (!u->is_same_dir(_u))
        res->fail("vec" + to_string(u->dim()) + ": is_same_dir");
    else
        res->pass();

    if (u->dim() == 4)
        res->inc_total(-1);
    else {
        if (!u->is_collinear(_u))
            res->fail("vec"+to_string(u->dim())+": is_collinear");
        else
            res->pass();
    }

    if (!u->is_perpendicular(w))
        res->fail("vec" + to_string(u->dim()) + ": is_perpendicular");
    else
        res->pass();

    delete _u;
}

void vec_raw_eq(test_res *res, const double *u, const double *v, int dim, const string &func)
{
    for (auto i = 0; i < dim; i++) {
        if (u[i] != v[i]) {
            res->fail("vec" + to_string(dim) + ": " + func);
            break;
        }

        if (i == dim - 1)
            res->pass();
    }
}