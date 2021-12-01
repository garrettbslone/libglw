//
// Created by Garrett on 9/28/2021.
//

#ifndef LIBGLW_WINDOW_HPP
#define LIBGLW_WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "../graphics/framebuffer.hpp"
#include "../graphics/graphics_context.hpp"
#include "../input/input.hpp"

#include <functional>

namespace glw {

class window;

struct window_spec {
    std::string title_{"glw app"};
    uint32_t width_{1280}, height_{720};
    bool fullscreen_{false}, maximized_{false};
    float alpha_{1.f};
};

/*
 * Callback function to be called upon closing the window.
 */
using close_cb = std::function<void(void)>;

struct window_data {
    close_cb close_;
    input *input_;
};

/*
 * An object that encapsulates a window to be displayed on the screen as well
 * as a graphics api context to render to.
 */
class window {
public:
    explicit window(const window_spec &spec);
    window(const window_spec &spec, const window_data &data);
    ~window();

    /*
     * Clear the screen through the rendering context.
     */
    void clear();
    /*
     * Force a window into fullscreen mode. If the window is already in fullscreen,
     * it will stay in fullscreen.
     */
    void force_fullscreen();
    /*
     * Resize the window. If the window is in fullscreen mode, this call has no
     * effect.
     */
    void resize(uint32_t width, uint32_t height);
    /*
     * Sets the close callback function to be called upon hitting the x arrow.
     */
    void set_close_cb(close_cb cb);
    /*
     * Set the window's title.
     */
    void set_title(const std::string& title);

    void set_window_data(const window_data &data);

    /*
     * Update the window. This should be called once per render pass.
     * Returns true if the window is closing or false otherwise.
     */
    bool update();

    /*
     * Returns a void pointer to the underlying native (OS) window
     * implementation.
     */
    void *get_native_window();

    inline framebuffer *get_framebuffer() const { return this->fb_; }

    inline double get_aspect_ratio() { return static_cast<double>(this->spec_.width_) / this->spec_.height_; }

protected:
    GLFWwindow *get_g_window();
    GLFWmonitor *get_monitor();
    void resize_framebuffer();

private:
    /*
     * GLFW framebuffer resize callback signature.
     */
    void resize_framebuffer(GLFWwindow *w, uint32_t width, uint32_t height);
    /*
     * GLFW window close callback signature.
     */
    void window_set_close(GLFWwindow *w);
    /*
     * Creates the underlying native (GLFW) window.
     */
    void create();

    const char *err_{nullptr};

    window_spec spec_;
    window_data data_;

    framebuffer *fb_;
    graphics_context *graphics_ctx_;
    color clear_clr_;

    GLFWwindow *native_window_;
    GLFWmonitor *monitor_;
};

}

#endif //LIBGLW_WINDOW_HPP
