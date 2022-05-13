//
// Created by Garrett on 3/18/22.
//

#ifndef GLW_SWAP_CHAIN_HPP
#define GLW_SWAP_CHAIN_HPP

#include "device.hpp"

namespace glw {

/*
 * An abstract swap chain object to handle the buffering of images to be displayed.
 * This can be ignored when using opengl as the api.
 */
class swap_chain {
public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    virtual ~swap_chain() = default;

    swap_chain(const swap_chain &) = delete;
    swap_chain &operator=(const swap_chain &) = delete;

protected:
    swap_chain() = default;
};

}

#endif //GLW_SWAP_CHAIN_HPP
