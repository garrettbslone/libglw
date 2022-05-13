//
// Created by Garrett on 3/18/22.
//

#ifndef GLW_PIPELINE_HPP
#define GLW_PIPELINE_HPP

#include <string>
#include <vector>

#include "device.hpp"
#include "shader.hpp"

namespace glw {

/*
 * An object used to configure the rendering pipeline through a pipeline object.
 * This can be ignored when using opengl as the api.
 */
class pipeline_config_info {
public:
    pipeline_config_info() = default;
    virtual ~pipeline_config_info() = default;

    /*
     * Not copyable or movable.
     */
    pipeline_config_info(const pipeline_config_info &) = delete;
    pipeline_config_info &operator=(const pipeline_config_info &) = delete;
};

/*
 * An abstract pipeline object to encapsulate a rendering pipeline.
 * This can be ignored when using opengl as the api.
 */
class pipeline {
public:
    virtual ~pipeline() = default;

    /*
     * Not copyable or movable.
     */
    pipeline(const pipeline &p) = delete;
    pipeline &operator=(const pipeline &p) = delete;

    inline shader *get_shader() const { return this->shader_; }
    inline void set_shader(shader *s) { this->shader_ = s; }

    /*
     * Create an api specific pipeline.
     */
    static pipeline *create(device *d,
            pipeline_config_info *info,
            const std::string &vert_path,
            const std::string &frag_path,
            graphics_api api);

private:
    /*
     * Pipelines in Vulkan have shaders baked in.
     */
    shader *shader_;
};

}

#endif //GLW_PIPELINE_HPP
