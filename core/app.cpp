//
// Created by Garrett on 10/30/2021.
//

#include "app.hpp"
#include "../util/exception.hpp"

namespace glw {

//declare app instance. Point to null
app *app::instance = nullptr;

app::app(const app_spec &spec)
{
    // check if an instance is already running
    if (instance)
        throw app_ex("app already exists!");

    // initialize app class members
    instance = this;

    this->spec_ = spec;
    this->input_ = new input();

    // create a new window object and set window params
    this->window_ = new window({
            spec.title_, spec.width_, spec.height_, spec.fullscreen_
    }, {
            nullptr, nullptr, nullptr, this->input_
    });

    // set app instance to running
    this->running_ = true;
    // window by default will not be minimized
    this->minimized_ = false;
}
// destroy app instance
app::~app()
{
    delete this->window_;
    delete this->input_;
}

void app::run()
{
    // clear any elements already running in the window.
    while (this->running_) {
        this->window_->clear();

        // update scene
        if (this->scene_)
            this->scene_->update();

        // if window is being updated, set instance to false
        if (this->window_->update())
            this->running_ = false;
    }
}
// stop the instance if app is closed
void app::close()
{
    this->running_ = false;
}

// define key and mouse behaviours
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

// change window size on resizing
void app::on_window_resize(resize_cb cb)
{
    this->window_->set_resize_cb(cb);
}

void app::attach_scene(scene *s)
{
    this->scene_ = s;
    this->scene_->attach();
}

}
