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
    this->input_ = new input();

    this->window_ = new window({
            spec.title_, spec.width_, spec.height_, spec.fullscreen_
    }, {
            nullptr, nullptr, nullptr, this->input_
    });
    this->running_ = true;
    this->minimized_ = false;
}

app::~app()
{
    delete this->window_;
    delete this->input_;
}

void app::run()
{
    while (this->running_) {
        this->window_->clear();

        if (this->scene_)
            this->scene_->update();

        if (this->window_->update())
            this->running_ = false;
    }
}

void app::close()
{
    this->running_ = false;
}

void app::on_key_down(key_down_cb cb)
{
    this->input_->key_down = cb;
}

void app::on_key_up(key_up_cb cb)
{
    this->input_->key_up = cb;
}

void app::on_mouse_move(mouse_move_cb cb)
{
    this->input_->mouse_move = cb;
}

void app::on_mouse_button_down(mouse_button_down_cb cb)
{
    this->input_->mouse_btn_down = cb;
}

void app::on_mouse_button_up(mouse_button_up_cb cb)
{
    this->input_->mouse_btn_up = cb;
}

void app::on_window_resize(resize_cb cb)
{
    this->window_->set_resize_cb(cb);
}

void app::attach_scene(scene *s)
{
    renderer *r = renderer::create(this->window_, nullptr, this->spec_.api_);

    this->scene_ = s;
    this->scene_->attach(r);
}

}
