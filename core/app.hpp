//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_APP_HPP
#define GLW_APP_HPP

#include "window.hpp"
#include "../input/input.hpp"
#include "../scene/scene.hpp"

namespace glw {

struct app_spec {
    std::string title_{"glw app"};
    uint32_t width_{1280}, height_{720};
    bool fullscreen_{false};
};

class app {
public:
    explicit app(const app_spec &spec);
    virtual ~app();

    void run();
    void close();

    void on_key_down(key_down_cb cb);
    void on_key_up(key_up_cb cb);
    void on_mouse_move(mouse_move_cb cb);
    void on_mouse_button_down(mouse_button_down_cb cb);
    void on_mouse_button_up(mouse_button_up_cb cb);

    void attach_scene(scene *s);

    inline window *get_window() { return this->window_; }

    inline static app &get() { return *app::instance; }

private:
    input *input_;
    window *window_;
    scene *scene_;
    app_spec spec_;
    bool running_, minimized_;

    static app *instance;
};

}

#endif //GLW_APP_HPP
