//
// Created by Garrett on 3/18/22.
//

#ifndef GLW_DEVICE_HPP
#define GLW_DEVICE_HPP

namespace glw {

class device {
public:
    virtual ~device() = default;

    // Not copyable or movable
    device(const device &) = delete;
    device &operator=(const device &) = delete;
    device(device &&) = delete;
    device &operator=(device &&) = delete;
};

}

#endif //GLW_DEVICE_HPP
