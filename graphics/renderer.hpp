//
// Created by Garrett on 3/18/22.
//

#ifndef GLW_RENDERER_HPP
#define GLW_RENDERER_HPP

#include "../core/window.hpp"

#include "buffer.hpp"
#include "graphics_api.hpp"
#include "pipeline.hpp"

namespace glw {

/*
 * An abstract renderer class that handles the rendering operations.
 * A renderer should be attached to each scene.
 */
class renderer {
public:
    virtual ~renderer() = default;

    /*
     * Non-copyable or movable.
     */
    renderer(const renderer &) = delete;
    renderer &operator=(const renderer &) = delete;

    inline bool is_frame_in_progress() const { return this->is_frame_started_; }

    /*
     * Returns the current frame that the renderer is targeting. Should be
     * ignored if using opengl backend.
     */
    inline int get_frame_index() const
    {
        return this->is_frame_started_ ? this->current_frame_index_ : -1;
    }

    /*
     * Initialize the renderer. Call this method after creating a renderer object.
     */
    virtual void init() = 0;

    /*
     * Return the aspect ratio of the viewport.
     */
    virtual float get_aspect_ratio() const  = 0;

    /*
     * Initialize the current frame in the renderer and retrieve the command buffer
     * to target.
     */
    virtual command_buffer *begin_frame() = 0;
    /*
     * End thd current frame after the render pass has been completed. Must be called
     * at the end of each frame render pass. Ensures that front buffer gets swapped out.
     */
    virtual void end_frame() = 0;

    /*
     * Begin a render pass on the swap chain, initializing the command buffer. This
     * must be called after and with the buffer reference returned from begin_frame.
     * This can be ignored when using opengl as the api.
     */
    virtual void begin_swap_chain_render_pass(command_buffer *cmd_buffer) = 0;
    /*
     * End the render pass on the swap chain that began during begin_swap_chain_render_pass.
     * This must be called after and with the same buffer reference that was passed to
     * begin_swap_chain_render_pass. This can be ignored when using opengl as the api.
     */
    virtual void end_swap_chain_render_pass(command_buffer *cmd_buffer) = 0;

    virtual void *get_descriptor_set() = 0;

    /*
     * Create an api specific renderer context.
     */
    static renderer *create(window *w, device *d, graphics_api api);

protected:
    /*
     * Base constructor needed for derived classes since the copy & move
     * constructors were deleted in the base class.
     */
    explicit renderer(window *w);

    /*
     * Helper functions to help allocate & deallocate the command buffers
     * attached to the renderer. These can be ignored when using opengl as
     * the api.
     */
    virtual void create_command_buffers();
    virtual void free_command_buffers();
    /*
     * Helper function to recreate the swap chain if it becomes invalidated.
     * This can be ignored when using opengl as the api.
     */
    virtual void recreate_swap_chain();

    window *window_;
    std::vector<command_buffer *> command_buffers_{};

    /*
     * Indices to keep track of which frame and image in the swap chain the
     * renderer is targeting. These can be ignored when using opengl as the api.
     */
    uint32_t current_image_index_{0};
    int current_frame_index_{0};
    bool is_frame_started_{false};
};

}

#endif //GLW_RENDERER_HPP
