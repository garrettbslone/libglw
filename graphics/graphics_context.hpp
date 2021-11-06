//
// Created by Garrett on 10/30/2021.
//

#ifndef GLW_GRAPHICS_CONTEXT_HPP
#define GLW_GRAPHICS_CONTEXT_HPP

namespace glw {

/*
 * An object that encapsulates a graphics API context to be handled by a window.
 */
class graphics_context {
public:
    virtual ~graphics_context() = default;

    virtual void init() = 0;
    /*
     * Swaps the front and back buffers for double buffered contexts.
     */
    virtual void swap_buffers() = 0;

    /*
     * Creates an implementation specific context to a graphics API.
     */
    static graphics_context *create(void *window);
};

}

#endif //GLW_GRAPHICS_CONTEXT_HPP
