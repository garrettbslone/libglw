//
// Created by Garrett on 3/29/22.
//

#include "vk_shader.hpp"
#include "../../util/exception.hpp"

namespace glw {

vk_shader::vk_shader(
        const std::string &name,
        const std::string &vertex_src,
        const std::string &fragment_src
)
{
    this->name = name;

    if (!(this->device_ = vk_device::get()))
        throw vulkan_device_ex("a vk_device is required to create a vk_shader!");

    auto code = shader::read_file(vertex_src);
    std::vector<char> code_vec = std::vector<char>(code.begin(), code.end());
    create_shader_module(code_vec, &this->vert_sm_, this->device_->device_);

    code = shader::read_file(fragment_src);
    code_vec = std::vector<char>(code.begin(), code.end());
    create_shader_module(code_vec, &this->frag_sm_, this->device_->device_);
}

vk_shader::~vk_shader()
{
    vkDestroyShaderModule(this->device_->device_, this->vert_sm_, nullptr);
    vkDestroyShaderModule(this->device_->device_, this->frag_sm_, nullptr);
}

void vk_shader::bind() const
{

}

void vk_shader::unbind() const
{

}

void vk_shader::set_int(const std::string &name, int val)
{

}

void vk_shader::set_int_arr(const std::string &name, int *vals, uint32_t cnt)
{

}

void vk_shader::set_float(const std::string &name, float val)
{

}

void vk_shader::set_float_arr(const std::string &name, float *vals, uint32_t cnt)
{

}

void vk_shader::set_float2(const std::string &name, const glm::vec2 &v)
{

}

void vk_shader::set_float3(const std::string &name, const glm::vec3 &v)
{

}

void vk_shader::set_float4(const std::string &name, const glm::vec4 &v)
{

}

void vk_shader::set_mat4(const std::string &name, const glm::mat4 &m)
{

}

void vk_shader::create_shader_module(const std::vector<char> &code,
        VkShaderModule *module,
        VkDevice device)
{
    VkShaderModuleCreateInfo create_info{};

    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t *>(code.data());

    if (vkCreateShaderModule(device, &create_info, nullptr, module) != VK_SUCCESS)
        throw vulkan_load_ex("failed to create shader module");
}

}