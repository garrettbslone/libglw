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

class pipeline_config_info {
public:
    pipeline_config_info(const pipeline_config_info &) = delete;
    pipeline_config_info &operator=(const pipeline_config_info &) = delete;
};

class pipeline {
public:
    virtual ~pipeline() = default;

    pipeline(const pipeline &p) = delete;
    pipeline &operator=(const pipeline &p) = delete;

    static pipeline *create(device *d,
            const pipeline_config_info &info,
            const std::string &vert_path,
            const std::string &frag_path);

    static void default_config_info(pipeline_config_info &info);

    inline shader *get_shader() const { return this->shader_; }
    inline void set_shader(shader *s) { this->shader_ = s; }

private:
    shader *shader_;
};

}

#endif //GLW_PIPELINE_HPP
