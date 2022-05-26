//
// Created by Garrett on 3/28/22.
//

#ifndef GLW_VK_DEVICE_HPP
#define GLW_VK_DEVICE_HPP

#include "../../ext/glad/vulkan.h"
#include "../device.hpp"

#include <string>
#include <vector>

namespace glw {

class swap_chain_support_details {
private:
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;

    friend class vk_device;
    friend class vk_swap_chain;
};

class queue_family_indices {
private:
    uint32_t graphics_family;
    uint32_t present_family;
    bool graphics_family_has_value = false;
    bool present_family_has_value = false;

    inline bool is_complete() const
    {
        return graphics_family_has_value && present_family_has_value;
    }

    friend class vk_device;
    friend class vk_swap_chain;
};

class vk_device : public device {
public:
    explicit vk_device(window *w);
    ~vk_device() override;

    // Not copyable or movable
    vk_device(const vk_device &) = delete;
    vk_device &operator=(const vk_device &) = delete;
    vk_device(vk_device &&) = delete;
    vk_device &operator=(vk_device &&) = delete;

private:
#ifdef NDEBUG
    const bool enable_validation_layers = false;
#else
    const bool enable_validation_layers = true;
#endif

    VkInstance get_instance() const { return this->instance_; }
    VkCommandPool get_command_pool() { return command_pool_; }
    VkDevice dev() { return device_; }
    VkSurfaceKHR surface() { return surface_; }
    VkQueue graphics_queue() { return graphics_queue_; }
    VkQueue present_queue() { return present_queue_; }

    inline swap_chain_support_details get_swap_chain_support()
    {
        return query_swap_chain_support(physical_device_);
    }
    inline queue_family_indices find_physical_queue_families()
    {
        return find_queue_families(physical_device_);
    }

    uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);
    VkFormat find_supported_format(
            const std::vector<VkFormat> &candidates,
            VkImageTiling tiling,
            VkFormatFeatureFlags features);

    // Buffer Helper Functions
    void create_buffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            VkBuffer &buffer,
            VkDeviceMemory &buffer_memory);
    VkCommandBuffer begin_single_time_commands();
    void end_single_time_commands(VkCommandBuffer command_buffer);
    void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
    void copy_buffer_to_image(
            VkBuffer buffer,
            VkImage image,
            uint32_t width,
            uint32_t height,
            uint32_t layer_count);

    void create_image_with_info(
            const VkImageCreateInfo &image_info,
            VkMemoryPropertyFlags properties,
            VkImage &image,
            VkDeviceMemory &image_memory);

    void create_instance();
    void setup_debug_messenger();
    void setup_debug_logger();
    void create_surface();
    void pick_physical_device();
    void create_logical_device();
    void create_command_pool();

    // helper functions
    bool is_device_suitable(VkPhysicalDevice device);
    std::vector<const char *> get_required_extensions();
    bool check_validation_layer_support();
    queue_family_indices find_queue_families(VkPhysicalDevice device);
    void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT &create_info);
    void has_GLFW_required_instance_extensions();
    bool check_device_extension_support(VkPhysicalDevice dev);
    swap_chain_support_details query_swap_chain_support(VkPhysicalDevice device);

    VkPhysicalDeviceProperties properties_;
    VkInstance instance_;
    VkDebugUtilsMessengerEXT debug_messenger_;
    VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
    VkCommandPool command_pool_;

    VkDevice device_;
    VkSurfaceKHR surface_;
    VkQueue graphics_queue_;
    VkQueue present_queue_;

    const std::vector<const char *> validation_layers_ = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char *> device_extensions_ = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    static vk_device *get() { return instance; }
    static vk_device *instance;

    friend class vk_buffer;
    friend class vk_descriptor_set_layout;
    friend class vk_descriptor_pool;
    friend class vk_descriptor_writer;
    friend class vk_framebuffer;
    friend class vk_index_buffer;
    friend class vk_pipeline;
    friend class vk_pipeline_manager;
    friend class vk_renderer;
    friend class vk_shader;
    friend class vk_swap_chain;
    friend class vk_vertex_buffer;
};

}

#endif //GLW_VK_DEVICE_HPP
