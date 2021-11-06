//
// Created by Garrett on 9/25/21.
//

#include <iostream>
#include <vector>

#include <glw.hpp>

#include "../include/test_res.hpp"

using namespace glw;

using std::cout;
using std::endl;
using std::string;
using std::to_string;

void mat_base_test(test_res *res, int dim, mat<double> *m);
void mat_arithmetic_test(test_res *res, mat<double> *m, mat<double> *n, mat<double> *o);
void mat_other_test(test_res *res, int det, mat<double> *m, mat<double> *n);
void mat3_operator_test(test_res *res, mat3<double> *m, mat3<double> *n, mat3<double> *o, double *inv);
void mat4_operator_test(test_res *res, mat4<double> *m, mat4<double> *n, mat4<double> *o, double *inv);
void mat_rot_test(test_res *res, int deg, mat<double> *m, mat<double> *n);
void mat_scale_test(test_res *res, mat<double> *m, mat<double> *n, vec3<double> *v);
void mat_translate_test(test_res *res, mat<double> *m, mat<double> *n, vec3<double> *v);
void mat4_projection_test(test_res *res, mat4<double> *m, mat4<double> *n);

void mat_raw_eq(test_res *res, const double *u, const double *v, int dim, int r, const string &func);

int main()
{
    double d1[] = {1, 2, 3,
                   0, 1, 4,
                   5, 6, 0};
    double d2[] = {4, 3, 1,
                   3, 0, 3,
                   -3, 4, 1};
    double d3[] = {5, 5, 4,
                   3, 1, 7,
                   2, 10, 1};

    mat3<double> *m3[3] = {
          new mat3<double>(d1),
          new mat3<double>(d2),
          new mat3<double>(d3)
    };

    double d4[] = {1, 1, 1, -1,
                   1, 1, -1, 1,
                   1, -1, 1, 1,
                   -1, 1, 1, 1};
    double d5[] = {5, 5, 3, 2,
                   6, 3, -1, -2,
                   -5, 4, 3, -1,
                   3, 5, 6, -7};
    double d6[] = {6, 6, 4, 1,
                   7, 4, -2, -1,
                   -4, 3, 4, 0,
                   2, 6, 7, -6};

    mat4<double> *m4[3] = {
            new mat4<double>(d4),
            new mat4<double>(d5),
            new mat4<double>(d6)
    };

    auto res = new test_res();

    cout << "starting...\n";
    cout << "basic...\n";

    mat_base_test(res, 9, m3[0]);
    mat_base_test(res, 16, m4[0]);

    (void) m3[0]->set(d1);
    (void) m4[0]->set(d4);

    cout << "arithmetic...\n";

    mat_arithmetic_test(res, m3[0]->clone(), m3[1]->clone(), m3[2]->clone());
    mat_arithmetic_test(res, m4[0]->clone(), m4[1]->clone(), m4[2]->clone());

    cout << "other...\n";

    double inv1[] = {-24, 18, 5,
                     20, -15, -4,
                     -5, 4, 1 };
    double inv2[] = {0.25, 0.25, 0.25, -0.25,
                     0.25, 0.25, -0.25, 0.25,
                     0.25, -0.25, 0.25, 0.25,
                     -0.25, 0.25, 0.25, 0.25};

    mat_other_test(res, 1, m3[0]->clone(), m3[1]->clone()->set(inv1));
    mat_other_test(res, -16, m4[0]->clone(), m4[1]->clone()->set(inv2));

    cout << "operators...\n";

    mat3_operator_test(res,
            dynamic_cast<mat3<double> *>(m3[0]->clone()),
            dynamic_cast<mat3<double> *>(m3[1]->clone()->set(d2)),
            dynamic_cast<mat3<double> *>(m3[2]->clone()),
               inv1);

    mat4_operator_test(res,
            dynamic_cast<mat4<double> *>(m4[0]->clone()),
            dynamic_cast<mat4<double> *>(m4[1]->clone()->set(d5)),
            dynamic_cast<mat4<double> *>(m4[2]->clone()),
               inv2);

    cout << "rotations...\n";

    mat_rot_test(res, 90,
            m3[0]->clone()->identity(),
            m3[1]->clone()->identity());
    mat_rot_test(res, 90,
            m4[0]->clone()->identity(),
            m4[1]->clone()->identity());

    cout << "scales...\n";

    auto v = new vec3<double>(3, 4, 5);

    mat_scale_test(res, m3[0]->clone()->identity(), m3[1]->clone()->identity(), v);
    mat_scale_test(res, m4[0]->clone()->identity(), m4[1]->clone()->identity(), v);

    cout << "translations...\n";

    mat_translate_test(res, m3[0]->clone()->identity(), m3[1]->clone()->identity(), v);
    mat_translate_test(res, m4[0]->clone()->identity(), m4[1]->clone()->identity(), v);

    cout << "projections...\n";

    mat4_projection_test(res,
            dynamic_cast<mat4<double> *>(m4[0]->clone()->identity()),
            dynamic_cast<mat4<double> *>(m4[1]->clone()->identity()));

    cout << *res;

    for (auto i = 0; i < 3; i++) {
        delete m3[i];
        delete m4[i];
    }

    return 0;
}

void mat_base_test(test_res *res, int dim, mat<double> *m)
{
    res->inc_total(6);

    if (dim != m->dim())
        res->fail("mat" + to_string(dim) + ": dim");
    else
        res->pass();

    switch (dim) {
    case 9: {
        double tmp[9] = {1, 2, 3,
                         0, 1, 4,
                         5, 6, 0 };
        mat_raw_eq(res, m->raw_data(), tmp, 9, m->rows(), "raw_data");

        break;
    }
    case 16:
    default: {
        double tmp[16] = {1, 1, 1, -1,
                          1, 1, -1, 1,
                          1, -1, 1, 1,
                          -1, 1, 1, 1};
        mat_raw_eq(res, m->raw_data(), tmp, 16, m->rows(), "raw_data");

        break;
    }
    }

    auto n = m->clone();

    for (auto i = 0; i < m->dim(); i++) {
        if (m->raw_data()[i] != n->raw_data()[i]) {
            res->fail("mat" + to_string(m->rows()) + ": clone");
            break;
        }

        if (i == m->dim() - 1) {
            n->raw_data()[0] *= 2.0;

            if (m->raw_data()[0] != n->raw_data()[0])
                res->pass();
            else
                res->fail("mat" + to_string(m->rows()) + ": clone");
        }
    }

    n->copy(m);

    mat_raw_eq(res, n->raw_data(), m->raw_data(), n->dim(), m->rows(), "copy");

    switch (dim) {
    case 9: {
        auto o = dynamic_cast<mat3<double> *>(n);
        double tmp[9] = {1, 2, 3,
                         0, 1, 4,
                         5, 6, 0};
        o->set(tmp);

        double t[9] = {1, 2, 3,
                       0, 1, 4,
                       5, 6, 0};

        mat_raw_eq(res, o->raw_data(), t, o->dim(), m->rows(), "set");

        break;
    }
    case 16: {
    default:auto o = dynamic_cast<mat4<double> *>(n);
        double tmp[16] = {1, 1, 1, -1,
                          1, 1, -1, 1,
                          1, -1, 1, 1,
                          -1, 1, 1, 1};

        o->set(tmp);

        double t[16] = {1, 1, 1, -1,
                        1, 1, -1, 1,
                        1, -1, 1, 1,
                        -1, 1, 1, 1};

        mat_raw_eq(res, o->raw_data(), t, o->dim(), m->rows(), "set");

        break;
    }
    }

    m->identity();

    for (auto i = 0; i < m->dim(); i++) {
        if (i % 4 == 0 && m->raw_data()[i] != 1.0 && m->raw_data()[i] != 0.0) {
            res->fail("mat" + to_string(sqrt(m->dim())) + ": identity");
            break;
        }

        if (i == m->dim() - 1)
            res->pass();
    }

    delete n;
}

void mat_arithmetic_test(test_res *res, mat<double> *m, mat<double> *n, mat<double> *o)
{
    const int num_tests = 6;

    res->inc_total(num_tests);

    if (m->dim() != n->dim() || m->dim() != o->dim() || n->dim() != o->dim()) {
        res->fail(num_tests, "matrices m, n, & o are not all of the same dimension");

        delete m;
        delete n;
        delete o;
        return;
    }

    auto _m = m->clone();

    m->add(n);

    mat_raw_eq(res, m->raw_data(), o->raw_data(), m->dim(), m->rows(), "add");

    m->sub(n);

    mat_raw_eq(res, m->raw_data(), _m->raw_data(), m->dim(), m->rows(), "add");

    delete _m;

    double tmp[o->dim()];

    for (auto i = 0; i < o->dim(); i++)
        tmp[i] = o->raw_data()[i] * 2.0;

    o->s_mul(2.0);

    mat_raw_eq(res, o->raw_data(), tmp, o->dim(), m->rows(), "s_mul");

    o->identity();
    vec<double> *v;

    if (o->dim() == 9)
        v = new vec3<double>(4.0, 24.4, 0.24);
    else
        v = new vec4<double>(4.0, 24.4, 0.24, -34.4);

    auto u = o->v_mul(v);

    for (auto i = 0; i < v->dim(); i++) {
        if (v->raw_data()[i] != u->raw_data()[i]) {
            res->fail("mat" + to_string(o->rows()) + ": v_mul");
            break;
        }

        if (i == v->dim() - 1)
            res->pass();
    }

    delete v;
    delete u;

    switch (m->dim()) {
    case 9: {
        tmp[0] = 1; tmp[1] = 15; tmp[2] = 10;
        tmp[3] = -9; tmp[4] = 16; tmp[5] = 7;
        tmp[6] = 38; tmp[7] = 15; tmp[8] = 23;

        auto mm = m->clone();
        m->l_mul(n);

        mat_raw_eq(res, m->raw_data(), tmp, m->dim(), m->rows(), "l_mul");

        tmp[0] = 9; tmp[1] = 17; tmp[2] = 24;
        tmp[3] = 18; tmp[4] = 24; tmp[5] = 9;
        tmp[6] = 2; tmp[7] = 4; tmp[8] = 7;

        m->copy(mm);
        m->r_mul(n);

        mat_raw_eq(res, m->raw_data(), tmp, m->dim(), m->rows(), "r_mul");

        delete mm;
        break;
    }
    case 16:
    default: {
        tmp[0] = 3; tmp[1] = 7; tmp[2] = -1; tmp[3] = 6;
        tmp[4] = 19; tmp[5] = 9; tmp[6] = 5; tmp [7] = -6;
        tmp[8] = -3; tmp[9] = 11; tmp[10] = 13; tmp[11] = -4;
        tmp[12] = -1; tmp[13] = 7; tmp[14] = 5; tmp[15] = -12;

        auto mm = m->clone();
        m->l_mul(n);

        mat_raw_eq(res, m->raw_data(), tmp, m->dim(), m->rows(), "l_mul");

        tmp[0] = 11; tmp[1] = 9; tmp[2] = 5; tmp[3] = 5;
        tmp[4] = 10; tmp[5] = 8; tmp[6] = 0; tmp [7] = -6;
        tmp[8] = 3; tmp[9] = -5; tmp[10] = -7; tmp[11] = 11;
        tmp[12] = 21; tmp[13] = -5; tmp[14] = -3; tmp[15] = 1;

        m->copy(mm);
        m->r_mul(n);

        mat_raw_eq(res, m->raw_data(), tmp, m->dim(), m->rows(), "r_mul");

        delete mm;
        break;
    }
    }
}

void mat_other_test(test_res *res, int det, mat<double> *m, mat<double> *n)
{
    res->inc_total(3);

    if (det != m->det())
        res->fail("mat" + to_string(m->rows()) + ": det");
    else
        res->pass();

    auto _m = m->clone();

    m->inverse();

    mat_raw_eq(res, m->raw_data(), n->raw_data(), m->dim(), m->rows(), "inverse");

    double tmp[_m->dim()];
    double t;

    for (auto i = 0; i < _m->dim(); i++)
        tmp[i] = _m->raw_data()[i];

    switch (_m->dim()) {
    case 9: {
        t = tmp[1];
        tmp[1] = tmp[3];
        tmp[3] = t;

        t = tmp[2];
        tmp[2] = tmp[6];
        tmp[6] = t;

        t = tmp[5];
        tmp[5] = tmp[7];
        tmp[7] = t;

        break;
    }
    case 16:
    default: {
        t = tmp[1];
        tmp[1] = tmp[4];
        tmp[4] = t;

        t = tmp[2];
        tmp[2] = tmp[8];
        tmp[8] = t;

        t = tmp[3];
        tmp[3] = tmp[12];
        tmp[12] = t;

        t = tmp[6];
        tmp[6] = tmp[9];
        tmp[9] = t;

        t = tmp[7];
        tmp[7] = tmp[13];
        tmp[13] = t;

        t = tmp[11];
        tmp[11] = tmp[14];
        tmp[14] = t;

        break;
    }
    }

    _m->transpose();

    mat_raw_eq(res, _m->raw_data(), tmp, _m->dim(), _m->rows(), "transpose");

    delete m;
    delete n;
    delete _m;
}

void mat3_operator_test(test_res *res, mat3<double> *m, mat3<double> *n, mat3<double> *o, double *inv)
{
    res->inc_total(13);

    auto *_m = dynamic_cast<mat3<double> *>(m->clone());

    if (*m == _m)
        res->pass();
    else
        res->fail("mat3: operator ==");

    if (*m != n)
        res->pass();
    else
        res->fail("mat3: operator !=");

    if (*(*m + n) == o)
        res->pass();
    else
        res->fail("mat3: operator +");

    if (*(*m - n) == _m)
        res->pass();
    else
        res->fail("mat3: operator -");

    *m += n;

    if (*m == o)
        res->pass();
    else
        res->fail("mat3: operator +=");

    *m -= n;

    if (*m == _m)
        res->pass();
    else
        res->fail("mat3: operator -=");

    double t[m->dim()];

    for (auto i = 0; i < m->dim(); i++)
        t[i] = m->raw_data()[i] * 2;

    *m * 2;

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "operator * (s_mul)");

    *m *= 0.5;

    if (*m == _m)
        res->pass();
    else
        res->fail("mat3: operator *=");

    mat_raw_eq(res, (!*m)->raw_data(), inv, m->dim(), m->rows(), "operator !");

    for (auto i = 0; i < m->dim(); i++)
        t[i] = m->raw_data()[i];

    for (auto i = 0; i < m->dim(); i++) {
        if (t[i] != (*m)[i]) {
            res->fail("mat3: operator[]");
            break;
        }

        if (i == m->dim() - 1)
            res->pass();
    }

    if (~*m == m->det())
        res->pass();
    else
        res->fail("mat3: operator~");

    if (-*m == m->transpose())
        res->pass();
    else
        res->fail("mat3: operator- (transpose)");

    *m = _m;

    mat_raw_eq(res, m->raw_data(), _m->raw_data(), m->dim(), m->rows(), "operator =");

    delete _m;
    delete m;
    delete n;
    delete o;
}

void mat4_operator_test(test_res *res, mat4<double> *m, mat4<double> *n, mat4<double> *o, double *inv)
{
    res->inc_total(13);

    auto *_m = dynamic_cast<mat4<double> *>(m->clone());

    if (*m == _m)
        res->pass();
    else
        res->fail("mat3: operator ==");

    if (*m != n)
        res->pass();
    else
        res->fail("mat3: operator !=");

    if (*(*m + n) == o)
        res->pass();
    else
        res->fail("mat3: operator +");

    if (*(*m - n) == _m)
        res->pass();
    else
        res->fail("mat3: operator -");

    *m += n;

    if (*m == o)
        res->pass();
    else
        res->fail("mat3: operator +=");

    *m -= n;

    if (*m == _m)
        res->pass();
    else
        res->fail("mat3: operator -=");

    double t[m->dim()];

    for (auto i = 0; i < m->dim(); i++)
        t[i] = m->raw_data()[i] * 2;

    *m * 2;

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "operator * (s_mul)");

    *m *= 0.5;

    if (*m == _m)
        res->pass();
    else
        res->fail("mat3: operator *=");

    mat_raw_eq(res, (!*m)->raw_data(), inv, m->dim(), m->rows(), "operator !");

    for (auto i = 0; i < m->dim(); i++)
        t[i] = m->raw_data()[i];

    for (auto i = 0; i < m->dim(); i++) {
        if (t[i] != (*m)[i]) {
            res->fail("mat3: operator[]");
            break;
        }

        if (i == m->dim() - 1)
            res->pass();
    }

    if (~*m == m->det())
        res->pass();
    else
        res->fail("mat3: operator~");

    if (-*m == m->transpose())
        res->pass();
    else
        res->fail("mat3: operator- (transpose)");

    *m = _m;

    mat_raw_eq(res, m->raw_data(), _m->raw_data(), m->dim(), m->rows(), "operator =");

    delete _m;
    delete m;
    delete n;
    delete o;
}

void mat_rot_test(test_res *res, int deg, mat<double> *m, mat<double> *n)
{
    res->inc_total(3);

    auto t = n->raw_data();

    if (m->dim() == 9) {
        t[4] = 0;
        t[5] = -1;
        t[7] = 1;
        t[8] = 0;
    } else {
        t[5] = 0;
        t[6] = -1;
        t[9] = 1;
        t[10] = 0;
    }

    m->rot_x(deg);

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "rot_x");

    m->identity();
    n->identity();

    t = n->raw_data();

    t[0] = 0;
    t[2] = 1;

    if (m->dim() == 9) {
        t[6] = -1;
        t[8] = 0;
    } else {
        t[8] = -1;
        t[10] = 0;
    }

    m->rot_y(deg);

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "rot_y");

    m->identity();
    n->identity();

    t = n->raw_data();

    t[0] = 0;
    t[1] = -1;

    if (m->dim() == 9) {
        t[3] = 1;
        t[4] = 0;
    } else {
        t[4] = 1;
        t[5] = 0;
    }

    m->rot_z(deg);

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "rot_z");

    delete m;
    delete n;
}

void mat_scale_test(test_res *res, mat<double> *m, mat<double> *n, vec3<double> *v)
{
    res->inc_total(2);

    auto t = n->raw_data();

    t[0] = v->x();

    if (m->dim() == 9) {
        t[4] = v->y();
        t[8] = v->z();
    } else {
        t[5] = v->y();
        t[10] = v->z();
    }

    m->scale(v);

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "scale (vec3)");

    m->identity();
    m->scale(v->x(), v->y(), v->z());

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "scale (x, y, z)");

    m->identity();
//    n->identity();

    if (m->dim() == 9) {
        res->inc_total(2);

        auto _m = dynamic_cast<mat3<double> *>(m);
        auto u = new vec2<double>(v->x(), v->y());

        t[8] = 1;

        _m->scale(u);

        mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "scale (vec2)");

        _m->identity();
        _m->scale(v->x(), v->y());

        mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "scale (x, y)");

        delete u;
    } else {
        res->inc_total(1);

        auto _m = dynamic_cast<mat4<double> *>(m);
        auto u = new vec4<double>(v->x(), v->y(), v->z(), v->z());

        _m->scale(u);

        mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "scale (vec4)");

        delete u;
    }

    delete m;
    delete n;
}

void mat_translate_test(test_res *res, mat<double> *m, mat<double> *n, vec3<double> *v)
{
    res->inc_total(4);

    auto t = n->raw_data();

    if (m->dim() == 9) {
        t[2] = v->x();
        t[5] = v->y();
        t[8] = v->z();
    } else {
        t[3] = v->x();
        t[7] = v->y();
        t[11] = v->z();
    }

    m->translate(v);

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "translate (vec3)");

    m->identity();
    m->translate(v->x(), v->y(), v->z());

    mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "translate (x, y, z)");

    m->identity();

    if (m->dim() == 9) {
        auto _m = dynamic_cast<mat3<double> *>(m);
        auto u = new vec2<double>(v->x(), v->y());

        t[8] = 1.0;

        _m->translate(u);

        mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "translate (vec2)");

        _m->identity();
        _m->translate(v->x(), v->y());

        mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "translate (x, y)");

        delete u;
    } else {
        auto _m = dynamic_cast<mat4<double> *>(m);
        auto u = new vec4<double>(v->x(), v->y(), v->z(), v->z());

        _m->translate(u);

        t[15] = v->z();

        mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "translate (vec4)");

        _m->identity();
        _m->translate(v->x(), v->y(), v->z(), v->z());

        mat_raw_eq(res, m->raw_data(), t, m->dim(), m->rows(), "translate (x, y, z, w)");

        delete u;
    }

    delete m;
    delete n;
}

void mat4_projection_test(test_res *res, mat4<double> *m, mat4<double> *n)
{
    res->inc_total(2);

    double p[] = {0.5, 0, 0, 0,
                  0, 1, 0, 0,
                  0, 0, -1.0202020406723022, -2.0202019214630127,
                  0, 0, -1, 0};

    n->set(p);
    m->perspective(90, 2, 1, 100);

    mat_raw_eq(res, m->raw_data(), n->raw_data(), m->dim(), m->rows(), "perspective");

    double o[] = {0.5, 0, 0, -0.5,
                  0, -2, 0, 3,
                  0, 0, -0.10000000149011612, -1.2999999523162842,
                  0, 0, 0, 1};

    m->identity();
    n->set(o);
    m->orthographic(-1, 3, 1, 2, 3, 23);

    mat_raw_eq(res, m->raw_data(), n->raw_data(), m->dim(), m->rows(), "orthographic");

    delete m;
    delete n;
}

bool round_eq(double u, double v) {
    return abs(abs(u) - abs(v)) <= TRIG_ROUND_ERR_FLT;
}

void mat_raw_eq(test_res *res, const double *u, const double *v, int dim, int r, const string &func)
{
    for (auto i = 0; i < dim; i++) {
        if (!round_eq(u[i], v[i])) {
            res->fail("mat" + to_string(r) + ": " + func);
            break;
        }

        if (i == dim - 1)
            res->pass();
    }
}