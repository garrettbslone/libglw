//
// Created by Garrett on 11/5/2021.
//

#include <cmath>
#include <random>
#include <iostream>
#include <map>
#include <glw.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "wormy.hpp"

using namespace glw;

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::map;

app *a;
vector<wormy *> worm;

auto cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

auto wormy_dir = glm::vec3(4.5f, 0.f, 0.f);
auto wormy_scale = glm::vec3(35.f, 35.f, 35.f);

const float CUBE_Z_TRANSLATION = -20.f;
int bg_width = 1280, bg_height = 900;
int score = 0;

void key_down(int k)
{
    switch ((key) k) {
        case KEY_UP:
            wormy_dir.x = 0.f;
            wormy_dir.y = -4.5f;
            break;
        case KEY_LEFT:
            wormy_dir.x = -4.5f;
            wormy_dir.y = 0.f;
            break;
        case KEY_RIGHT:
            wormy_dir.x = 4.5f;
            wormy_dir.y = 0.f;
            break;
        case KEY_DOWN:
            wormy_dir.x = 0.f;
            wormy_dir.y = 4.5f;
            break;
        case KEY_ESCAPE:
            a->close();
            break;
    }
}

void wrap(glm::vec3 w_pos)
{
    glm::vec3 t(0.f);

    // wrap off right -> left
    if (w_pos.x >= bg_width / 100.f)
        t.x = -2.85f * w_pos.x;
    // wrap off left -> right
    else if (w_pos.x <= (0.f - bg_width) / 100.f)
        t.x = -2.85f * w_pos.x;

    // wrap off bottom -> top
    if (w_pos.y >= bg_height / 100.f)
        t.y = -2.85f * w_pos.y;

    // wrap off top -> bottom
    else if (w_pos.y <= (0.f - bg_height) / 100.f)
        t.y = -2.85f * w_pos.y;

    worm[0]->translate(t);
}

glm::vec3 spaw_random_cube()
{
    int w = bg_width / 50;
    int h = bg_height / 50;

    return {((rand() % w) - w / 2),
        ((rand() % h) - h / 2),
            CUBE_Z_TRANSLATION};
}

bool are_overlapping(glm::vec3 w_pos, glm::vec3 c_pos)
{
    return abs(abs(w_pos.x) - abs(c_pos.x)) <= 1.f &&
           abs(abs(w_pos.y) - abs(c_pos.y)) <= 1.f;
}

int main(int argc, char *argv[])
{
    srand(time(nullptr));

    try {
        app_spec spec = {
            "test app",
            static_cast<uint32_t>(bg_width),
            static_cast<uint32_t>(bg_height),
            false
        };
        a = new app(spec);
        a->on_key_down(key_down);

        auto *s = new scene;
        a->attach_scene(s);

        vector<cube *> cubes(1, new cube());
        auto background = new quad();
        worm.push_back(new wormy(64));

        auto t = spaw_random_cube();
        cubes[0]->translate(t);

        map<pair<float, float>, cube *> cube_map;
        cube_map.insert({{t.x, t.y}, cubes[0]});

        background->translate(glm::vec3(0.f, 0.f, -75.f));
        worm[0]->translate(glm::vec3(0.f, 0.f, -10.f));

        s->push_node(background);
        s->push_node(worm[0]);
        s->push_node(cubes[0]);

        auto cube_sh = shader::create("cube", "vs.glsl", "fs.glsl");
        cubes[0]->set_shader(cube_sh);

        background->scale(glm::vec3(100.f, 100.f, 100.f));
        worm[0]->scale(wormy_scale);

        cube_sh->bind();
        auto proj = glm::perspective(glm::radians(45.f), (float) a->get_window()->get_aspect_ratio(), 0.1f, 100.f);
        cube_sh->set_mat4("proj", proj);

        auto wormy_sh = shader::create("wormy", "wormy.vs.glsl", "wormy.fs.glsl");
        worm[0]->set_shader(wormy_sh);
        wormy_sh->bind();
        wormy_sh->set_mat4("proj", glm::ortho(-((float) a->get_window()->get_width() / 2.0f),
                                              (float) a->get_window()->get_width() / 2.0f,
                                              (float) a->get_window()->get_height() / 2.0f,
                                              -((float) a->get_window()->get_height() / 2.0f),
                                              -1000.0f,
                                              1000.0f));

        auto board_sh = shader::create("board", "board.vs.glsl", "board.fs.glsl");
        background->set_shader(board_sh);
        board_sh->bind();
        board_sh->set_mat4("proj", proj);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        auto prev_time = (float) glfwGetTime();
        auto start_time = prev_time;

        a->on_window_resize([&proj, &wormy_sh, &cube_sh](int width, int height)
        {
            proj = glm::perspective(glm::radians(45.f), (float) a->get_window()->get_aspect_ratio(), 0.1f, 100.f);
            cube_sh->bind();
            cube_sh->set_mat4("proj", proj);

            wormy_sh->bind();
            wormy_sh->set_mat4("proj", glm::ortho(-((float) width / 2.0f),
                                                  (float) width / 2.0f,
                                                  (float) height / 2.0f,
                                                  -((float) height / 2.0f),
                                                  -1000.0f,
                                                  1000.0f));

            bg_width = width;
            bg_height = height;
        });

        s->on_update([&prev_time, &start_time, &s, &cubes, &cube_map, &cube_sh](const std::vector<drawable *> &nodes)
        {
            auto time = (float) glfwGetTime();
            auto dt = time - prev_time;
            prev_time = time;

            glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            for (auto n: nodes) {
                if (dynamic_cast<cube *>(n))
                    n->rotate(0.045f, glm::vec3(1.f, 0.3f, 0.5f));
                else if (dynamic_cast<wormy *>(n))
                    n->translate(wormy_dir * dt);

                auto _sh = n->get_shader();
                _sh->bind();

                _sh->set_float("time", time);
                _sh->set_mat4("view", view);
                _sh->set_mat4("model", n->get_model_mat());
                n->draw();
            }

            auto w_pos = worm[0]->get_position() / 50.f;
            vector<pair<int, int>> found;

            for (const auto it: cube_map) {
                auto c_pos = it.second->get_position();

                if (are_overlapping(w_pos, c_pos)) {
                    if (++score > 5)
                        a->close();

                    found.emplace_back(it.first);

                    auto _it = std::find(cubes.begin(), cubes.end(), it.second);
                    if (_it != cubes.end())
                        cubes.erase(_it);

                    s->pop_node(it.second);
                }
            }

            for (auto p: found) {
                cube_map.erase(p);
            }

            wrap(w_pos);

            if (time - start_time >= 5) {
                auto p = spaw_random_cube();

                while (cube_map.find({p.x, p.y}) != cube_map.end()) {
                    p = spaw_random_cube();
                }

                auto c = new cube();
                c->translate(p);
                cube_map.insert({{p.x, p.y}, c});
                c->set_shader(cube_sh);
                cubes.push_back(c);
                s->push_node(c);

                start_time = time;
            }
        });

        a->run();
        delete a;
        delete s;
        delete worm[0];
        for (auto c: cubes)
            delete c;
    }
    catch (glw_runtime_ex &ex) {
        cout << ex.msg << endl;
    }

    return 0;
}