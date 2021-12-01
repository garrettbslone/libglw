//
// Created by Garrett on 10/30/2021.
//

#include "app.hpp"
#include "../util/exception.hpp"

namespace glw {

app *app::instance = nullptr;

app::app(const app_spec &spec)
{
    if (instance)
        throw app_ex("app already exists!");

    instance = this;

    this->spec_ = spec;

    this->window_ = new window({
        spec.title_, spec.width_, spec.height_, spec.fullscreen_
    });
    this->running_ = true;
    this->minimized_ = false;
}

app::~app()
{
    delete this->window_;
}

void app::run()
{
    while (this->running_) {
        this->window_->clear();

        if (this->window_->update())
            this->running_ = false;
    }
}

void app::close()
{
    this->running_ = false;
}

}
