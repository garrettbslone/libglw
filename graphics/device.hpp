//
// Created by Garrett on 3/18/22.
//

#ifndef GLW_DEVICE_HPP
#define GLW_DEVICE_HPP

#include "graphics_api.hpp"

namespace glw {

class window;

/*
 * An abstract device class to handle interactions with CPU/GPU devices.
 * This can be ignored when using opengl as the api.
 */
class device {
public:
    virtual ~device() = default;

    // Not copyable or movable
    device(const device &) = delete;
    device &operator=(const device &) = delete;
    device(device &&) = delete;
    device &operator=(device &&) = delete;

    static device *create(window *w, graphics_api api);

protected:
    device(window *w);

    window *window_;
};

}

#endif //GLW_DEVICE_HPP
