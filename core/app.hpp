//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_APP_HPP
#define GLW_APP_HPP

#include "window.hpp"
#include "../input/input.hpp"
#include "../scene/scene.hpp"

namespace glw {
/*
 * struct for default app specs
 * set default app size to 1280*720 pixels
 * set default title
 * by default app will not occupy full screen
 */
struct app_spec {
    std::string title_{"glw app"};
    uint32_t width_{1280}, height_{720};
    bool fullscreen_{false};
};

class app {
public:
    // app constructor
    explicit app(const app_spec &spec);
    // app destructor
    virtual ~app();

    void run();
    void close();

    /*
     * methods from input.hpp
     * methods to define mouse and key movements
     */
    void on_key_down(key_down_cb cb);
    void on_key_up(key_up_cb cb);
    void on_mouse_move(mouse_move_cb cb);
    void on_mouse_button_down(mouse_button_down_cb cb);
    void on_mouse_button_up(mouse_button_up_cb cb);

    void on_window_resize(resize_cb cb);

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
