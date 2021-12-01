//
// Created by Garrett on 11/5/2021.
//

#include <iostream>
#include <glw.hpp>

using namespace glw;

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    try {
        app_spec spec = {
            "test app", 900, 900, false
        };
        auto a = new app(spec);
        a->run();
        delete a;

    } catch (glw_runtime_ex &ex) {
        cout << ex.msg << endl;
    }

    return 0;
}