//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_APP_HPP
#define GLW_APP_HPP

#include "../graphics/window.hpp"

namespace glw {

struct app_spec {
    std::string title_{"glw app"};
    uint32_t width_{1280}, height_{720};
    bool fullscreen_{false};
};

class app {
public:
    app(const app_spec &spec);
    virtual ~app();

    void run();
    void close();

    inline window *get_window() { return this->window_; }

    inline static app &get() { return *app::instance; }

private:
    window *window_;
    app_spec spec_;
    bool running_, minimized_;

    static app *instance;
};

}

#endif //GLW_APP_HPP
