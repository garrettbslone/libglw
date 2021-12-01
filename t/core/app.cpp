//
// Created by Garrett on 11/5/2021.
//

#include <iostream>
#include <glw.hpp>

using namespace glw;

using std::cout;
using std::endl;

void mouse_mv(double x, double y)
{
    cout << "Mouse: (" << x << ", " << y << ")\n";
}

void key_down(int k)
{
    if ((key) k == KEY_A) {
        cout << "A pressed\n";
    }
}

int main(int argc, char *argv[])
{
    try {
        app_spec spec = {
            "test app", 900, 900, false
        };
        auto a = new app(spec);
        a->on_mouse_move(mouse_mv);
        a->on_key_down(key_down);
        a->run();
        delete a;

    } catch (glw_runtime_ex &ex) {
        cout << ex.msg << endl;
    }

    return 0;
}