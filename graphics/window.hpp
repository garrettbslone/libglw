//
// Created by Garrett on 9/28/2021.
//

#ifndef LIBGLW_WINDOW_HPP
#define LIBGLW_WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include "../ext/glfw/include/GLFW/glfw3.h"

#include "framebuffer.hpp"
#include "graphics_context.hpp"

namespace glw {

#define WINDOW_MODE_FULLSCREEN (short) 1
#define WINDOW_MODE_MAXIMIZED (short) 2

/*
 * Callback function to be called upon closing the window.
 */
typedef void (*close_cb)();

/*
 * An object that encapsulates a window to be displayed on the screen as well
 * as a graphics api context to render to.
 */
class window {
public:
    window();
    /*
     * Creates a square window of dim x dim.
     */
    explicit window(int dim);
    /*
     * Creates either a fullscreen window or a maximized windowed mode window.
     * 'mode' should be either WINDOW_MODE_FULLSCREEN or WINDOW_MODE_MAXIMIZED.
     * If another value is passed as the 'mode', a viewport_ex exception will
     * be thrown.
     */
    explicit window(short mode);
    window(int dim, const std::string &title);
    window(short mode, const std::string &title);
    window(int width, int height);
    window(int width, int height, const std::string &title);
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
    void resize(int width, int height);
    /*
     * Sets the close callback function to be called upon hitting the x arrow.
     */
    void set_close_cb(close_cb cb);
    /*
     * Set the window's title.
     */
    void set_title(const std::string& title);
    /*
     * Update the window. This should be called once per render pass.
     */
    void update();

    /*
     * Returns a void pointer to the underlying native (OS) window
     * implementation.
     */
    void *get_native_window();
    framebuffer *get_framebuffer() const { return this->fb; }

protected:
    GLFWwindow *get_g_window();
    GLFWmonitor *get_monitor();
    void resize_framebuffer();

private:
    /*
     * GLFW framebuffer resize callback signature.
     */
    void resize_framebuffer(GLFWwindow *w, int width, int height);
    /*
     * GLFW window close callback signature.
     */
    void window_set_close(GLFWwindow *w);
    /*
     * Creates the underlying native (GLFW) window.
     */
    void create();

    int width, height;
    bool maximize, fullscreen;
    std::string title;
    const char *err{nullptr};
    float a = 1.f;

    framebuffer *fb;
    graphics_context *ctx;
    color clear_clr;

    close_cb c_cb;
    GLFWwindow *g_window;
    GLFWmonitor *monitor;
};

}

#endif //LIBGLW_WINDOW_HPP
