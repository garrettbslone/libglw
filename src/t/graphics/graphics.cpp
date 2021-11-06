//
// Created by Garrett on 9/28/2021.
//

#include <iostream>
#include <glw.hpp>

#include "../include/test_res.hpp"

using namespace glw;

using std::cout;
using std::endl;

int main()
{
    auto res = new test_res();
    res->inc_total(4);

    cout << "500 x 500 window...\n";

    try {
        auto *w = new window(500, 500);
        w->update();
        delete w;
        res->pass();
    } catch (viewport_ex &ex) {
        res->fail(ex.msg);
    }

    cout << "fullscreen window...\n";

    try {
        auto w2 = new window(WINDOW_MODE_MAXIMIZED, "Project 03 Playtest Iteration 1");
        w2->update();
        delete w2;
        res->pass();
    } catch (viewport_ex &ex) {
        res->fail(ex.msg);
    }

    cout << "orange texture...\n";

    try {
        auto t = texture::create("orange.png");
        delete t;
        res->pass();
    } catch (gl_load_ex &ex) {
        res->fail(ex.msg);
    }

    cout << "simple shader...\n";

    try {
        auto s = shader::create("quad", "vs.glsl", "fs.glsl");
        s->bind();
        delete s;
        res->pass();
    } catch (gl_load_ex &ex) {
        res->fail(ex.msg);
    }

    cout << *res << endl;
    delete res;
    return  0;
}