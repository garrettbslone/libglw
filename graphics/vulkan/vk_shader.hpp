//
// Created by Garrett on 3/29/22.
//

#ifndef GLW_VK_SHADER_HPP
#define GLW_VK_SHADER_HPP


#include "../shader.hpp"
#include "vk_device.hpp"

namespace glw {

class vk_pipeline;

class vk_shader : public shader {
public:
    vk_shader(const std::string &name,
            const std::string &vertex_src,
            const std::string &fragment_src,
            device *dev);
    ~vk_shader() override;

    void bind() const override;
    void unbind() const override;

    void set_int(const std::string &name, int val) override;
    void set_int_arr(const std::string &name, int *vals, uint32_t cnt) override;
    void set_float(const std::string &name, float val) override;
    void set_float_arr(const std::string &name, float *vals, uint32_t cnt) override;
    void set_float2(const std::string &name, const glm::vec2 &v) override;
    void set_float3(const std::string &name, const glm::vec3 &v) override;
    void set_float4(const std::string &name, const glm::vec4 &v) override;
    void set_mat4(const std::string &name, const glm::mat4 &m) override;

private:
    static void create_shader_module(const std::vector<char> &code, VkShaderModule *module, VkDevice device);

    VkShaderModule vert_sm_, frag_sm_;
    vk_device *device_;

    friend class vk_pipeline;
};

}

#endif //GLW_VK_SHADER_HPP
