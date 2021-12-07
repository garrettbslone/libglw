//
// Created by Garrett on 11/5/2021.
//

#include <random>
#include <iostream>
#include <glw.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glw;

using std::cout;
using std::endl;

app *a;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

void mouse_mv(double x, double y)
{
//    cout << "Mouse: (" << x << ", " << y << ")\n";
}

void key_down(int k)
{
    switch ((key) k) {
        case KEY_UP:
            cameraPos += glm::vec3(0.f, 0.25f, 0.f);
            break;
        case KEY_LEFT:
            cameraPos += glm::vec3(-0.25f, 0.f, 0.f);
            break;
        case KEY_RIGHT:
            cameraPos += glm::vec3(0.25f, 0.f, 0.f);
            break;
        case KEY_DOWN:
            cameraPos += glm::vec3(0.f, -0.25f, 0.f);
            break;
    }

    cout << "Key pressed: " << (key) k << endl;
}

void on_click(int btn)
{
    cout << "Mouse button clicked: " << btn << endl;

    auto c = a->get_window()->get_clear_clr();
    auto d = c.raw_data();

    d[rand() % 4] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    c.set(d);
    a->get_window()->set_clear_clr(c);
}

int main(int argc, char *argv[])
{
    srand(time(0));

    try {
        app_spec spec = {
            "test app", 1280, 900, false
        };
        a = new app(spec);
        a->on_mouse_move(mouse_mv);
        a->on_key_down(key_down);
        a->on_mouse_button_down(on_click);

        auto *s = new scene;
        a->attach_scene(s);

        auto c = new cube();
        auto cc = new cube();
        auto ccc = new cube();
        auto cccc = new cube();

        c->translate(glm::vec3(-2.f, -2.f, -5.f));
        cc->translate(glm::vec3(-2.f, 2.f, -5.f));
        ccc->translate(glm::vec3(2.f, -2.f, -5.f));
        cccc->translate(glm::vec3(2.f, 2.f, -5.f));

        s->push_node(c);
        s->push_node(cc);
        s->push_node(ccc);
        s->push_node(cccc);

        auto sh = shader::create("cube", "vs.glsl", "fs.glsl");
        c->set_shader(sh);
        cc->set_shader(sh);
        ccc->set_shader(sh);
        cccc->set_shader(sh);
//        c->set_model_mat(glm::scale(glm::mat4(1.f), glm::vec3(100.f, 100.f, 100.f)));
        sh->bind();
        auto proj = glm::perspective(glm::radians(45.f), (float) a->get_window()->get_aspect_ratio(), 0.1f, 100.f);
        sh->set_mat4("proj", proj);

        auto t = texture::create("wall.jpg");
        t->bind();
        sh->set_int("texture1", 0);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        s->on_update([t, sh](const std::vector<drawable *> &nodes)
        {
            t->bind(0);
            sh->bind();
            sh->set_float("time", (float) glfwGetTime());

            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            sh->set_mat4("view", view);

            int i = 0;

            for (auto n: nodes) {
                n->rotate(0.045f, glm::vec3(1.f, 0.3f, 0.5f));
                sh->set_mat4("model", n->get_model_mat());
                n->draw();
            }
        });

        a->run();
        delete a;
        delete s;
        delete c;

    } catch (glw_runtime_ex &ex) {
        cout << ex.msg << endl;
    }

    return 0;
}