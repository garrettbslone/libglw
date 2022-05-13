//
// Created by Garrett on 3/28/22.
//

#define GLAD_VULKAN_IMPLEMENTATION
#include "vk_device.hpp"
#include "../../util/exception.hpp"
#include "../../core/window.hpp"

#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

namespace glw {

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_cb(
        VkDebugReportFlagsEXT                       flags,
        VkDebugReportObjectTypeEXT                  objectType,
        uint64_t                                    object,
        size_t                                      location,
        int32_t                                     messageCode,
        const char *                                pLayerPrefix,
        const char *                                pMessage,
        void *                                      pUserData)
{
    std::string report = std::to_string( objectType ) +
            std::to_string( object ) + ": " +
            std::to_string( messageCode ) + ", " +
            pLayerPrefix + ", " +
            pMessage;

    switch (flags) {
    case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
        std::cout << "Info: " << report << std::endl;
        break;

    case VK_DEBUG_REPORT_WARNING_BIT_EXT:
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        std::cout << "WARNING: " << report << std::endl;
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        break;

    case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        std::cout << "PERFORMANCE: " << report << std::endl;
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        break;

    case VK_DEBUG_REPORT_ERROR_BIT_EXT:
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        std::cout << "ERROR: " << report << std::endl;
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
        break;

    case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
        report += "Debug: ";
        break;
    }

    // no abort on misbehaving command
    return VK_FALSE;
}

VkResult CreateDebugReportCallbackEXT(
        VkInstance                                  instance,
        const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDebugReportCallbackEXT*                   pCallback)
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(
            instance,
            "vkCreateDebugReportCallbackEXT");

    return func ? func(instance, pCreateInfo, pAllocator, pCallback) :
        VK_ERROR_EXTENSION_NOT_PRESENT;
}

// local callback functions
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

VKAPI_ATTR void VKAPI_CALL DestroyDebugReportCallbackEXT(
        VkInstance instance,
        VkDebugReportCallbackEXT callback,
        const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(
            instance,
            "vkDestroyDebugReportCallbackEXT");

    if (func)
        func(instance, callback, pAllocator);
}

VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkCreateDebugUtilsMessengerEXT");

    return func ? func(instance, pCreateInfo, pAllocator, pDebugMessenger) :
           VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance,
            "vkDestroyDebugUtilsMessengerEXT");

    if (func)
        func(instance, debugMessenger, pAllocator);
}

// class member functions
vk_device::vk_device(window *w)
        : device(w)
{
    create_instance();
    setup_debug_messenger();
    create_surface();
    pick_physical_device();
    create_logical_device();
    create_command_pool();
    setup_debug_logger();
}

vk_device::~vk_device() {
    vkDestroyCommandPool(this->device_, this->command_pool_, nullptr);
    vkDestroyDevice(this->device_, nullptr);

    if (enable_validation_layers) {
        DestroyDebugUtilsMessengerEXT(this->instance_,
                this->debug_messenger_,
                nullptr);
    }

    vkDestroySurfaceKHR(this->instance_, this->surface_, nullptr);
    vkDestroyInstance(this->instance_, nullptr);
}

void vk_device::create_instance() {
    if (enable_validation_layers && !check_validation_layer_support()) {
        throw vulkan_debug_layers_ex("validation layers requested, but not available!");
    }

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "LittleVulkanEngine App";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    auto extensions = get_required_extensions();
    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
    if (this->enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(this->validation_layers_.size());
        create_info.ppEnabledLayerNames = this->validation_layers_.data();

        populate_debug_messenger_create_info(debug_create_info);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debug_create_info;
    } else {
        create_info.enabledLayerCount = 0;
        create_info.pNext = nullptr;
    }

    if (vkCreateInstance(&create_info, nullptr, &this->instance_) != VK_SUCCESS) {
        throw vulkan_load_ex("failed to create vulkan instance!");
    }

    has_GLFW_required_instance_extensions();
}

void vk_device::pick_physical_device() {
    uint32_t device_cnt = 0;

    vkEnumeratePhysicalDevices(this->instance_, &device_cnt, nullptr);

    if (device_cnt == 0)
        throw vulkan_device_ex("failed to find GPU(s) with Vulkan support!");

    std::cout << "Device count: " << device_cnt << std::endl;
    std::vector<VkPhysicalDevice> devices(device_cnt);
    vkEnumeratePhysicalDevices(this->instance_, &device_cnt, devices.data());

    for (const auto &device : devices) {
        if (is_device_suitable(device)) {
            this->physical_device_ = device;
            break;
        }
    }

    if (this->physical_device_ == VK_NULL_HANDLE)
        throw vulkan_device_ex("failed to find a suitable GPU!");

    vkGetPhysicalDeviceProperties(this->physical_device_, &this->properties_);
    std::cout << "physical device: " << this->properties_.deviceName << std::endl;
}

void vk_device::create_logical_device() {
    queue_family_indices indices = find_queue_families(physical_device_);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphics_family,
            indices.present_family
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queue_create_info{};

        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queueFamily;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queuePriority;

        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    createInfo.pQueueCreateInfos = queue_create_infos.data();

    createInfo.pEnabledFeatures = &device_features;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(this->device_extensions_.size());
    createInfo.ppEnabledExtensionNames = this->device_extensions_.data();

    // might not really be necessary anymore because device specific validation layers
    // have been deprecated
    if (enable_validation_layers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(this->validation_layers_.size());
        createInfo.ppEnabledLayerNames = this->validation_layers_.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(this->physical_device_, &createInfo, nullptr, &this->device_) != VK_SUCCESS)
        throw vulkan_device_ex("failed to create logical device!");

    vkGetDeviceQueue(this->device_, indices.graphics_family, 0, &this->graphics_queue_);
    vkGetDeviceQueue(this->device_, indices.present_family, 0, &this->present_queue_);
}

void vk_device::create_command_pool() {
    queue_family_indices queue_family_indices = find_physical_queue_families();

    VkCommandPoolCreateInfo pool_info{};

    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = queue_family_indices.graphics_family;
    pool_info.flags =
            VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(this->device_, &pool_info, nullptr, &this->command_pool_) != VK_SUCCESS)
        throw vulkan_device_ex("failed to create command pool_ on the device!");
}

void vk_device::create_surface() {
    if (glfwCreateWindowSurface(this->instance_,
            reinterpret_cast<GLFWwindow *>(this->window_->get_native_window()),
            nullptr,
            &this->surface_) != VK_SUCCESS)
        throw vulkan_device_ex("Failed to create window surface");
}

bool vk_device::is_device_suitable(VkPhysicalDevice device) {
    queue_family_indices indices = find_queue_families(device);

    bool extensions_supported = check_device_extension_support(device);
    bool swap_chain_adequate = false;

    if (extensions_supported) {
        swap_chain_support_details support = query_swap_chain_support(device);
        swap_chain_adequate = !support.formats.empty() &&
                !support.present_modes.empty();
    }

    VkPhysicalDeviceFeatures supported_features;
    vkGetPhysicalDeviceFeatures(device, &supported_features);

    return indices.is_complete() && extensions_supported && swap_chain_adequate &&
            supported_features.samplerAnisotropy;
}

void vk_device::populate_debug_messenger_create_info(
        VkDebugUtilsMessengerCreateInfoEXT &create_info) {
    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debugCallback;
    create_info.pUserData = nullptr;  // Optional
}

void vk_device::setup_debug_messenger() {
    if (!enable_validation_layers) return;

    VkDebugUtilsMessengerCreateInfoEXT create_info;
    populate_debug_messenger_create_info(create_info);

    if (CreateDebugUtilsMessengerEXT(this->instance_,
            &create_info,
            nullptr,
            &this->debug_messenger_) != VK_SUCCESS)
        throw vulkan_debug_layers_ex("failed to set up debug messenger!");
}

void vk_device::setup_debug_logger()
{
    if (!enable_validation_layers) return;

    VkDebugReportCallbackCreateInfoEXT debug{};

    debug.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    debug.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    debug.pfnCallback = debug_cb;

    if (CreateDebugReportCallbackEXT(this->instance_,
            &debug,
            nullptr,
            reinterpret_cast<VkDebugReportCallbackEXT *>(debug_cb)) != VK_SUCCESS)
        throw vulkan_debug_layers_ex("failed to setup debug report!");
}

bool vk_device::check_validation_layer_support() {
    uint32_t layer_cnt;
    vkEnumerateInstanceLayerProperties(&layer_cnt, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_cnt);
    vkEnumerateInstanceLayerProperties(&layer_cnt, available_layers.data());

    for (const char *layer_name : this->validation_layers_) {
        bool layer_found = false;

        for (const auto &layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if (!layer_found)
            return false;
    }

    return true;
}

std::vector<const char *> vk_device::get_required_extensions() {
    uint32_t glfw_extension_cnt = 0;
    const char **glfw_extensions;

    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_cnt);
    std::vector<const char *> extensions(glfw_extensions,
            glfw_extensions + glfw_extension_cnt);

    if (enable_validation_layers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

void vk_device::has_GLFW_required_instance_extensions() {
    uint32_t extension_cnt = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_cnt, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_cnt);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_cnt, extensions.data());

    std::cout << "available extensions:" << std::endl;
    std::unordered_set<std::string> available;

    for (const auto &extension : extensions) {
        std::cout << "\t" << extension.extensionName << std::endl;
        available.insert(extension.extensionName);
    }

    std::cout << "required extensions:" << std::endl;
    auto required_extensions = get_required_extensions();

    for (const auto &required : required_extensions) {
        std::cout << "\t" << required << std::endl;

        if (available.find(required) == available.end())
            throw vulkan_device_ex("Missing required glfw extension: " +
                std::string(required));
    }
}

bool vk_device::check_device_extension_support(VkPhysicalDevice dev) {
    uint32_t extension_cnt;
    vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_cnt, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_cnt);
    vkEnumerateDeviceExtensionProperties(
            dev,
            nullptr,
            &extension_cnt,
            available_extensions.data());

    std::set<std::string> required_extensions(this->device_extensions_.begin(),
            this->device_extensions_.end());

    for (const auto &extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

queue_family_indices vk_device::find_queue_families(VkPhysicalDevice device) {
    queue_family_indices indices;

    uint32_t queue_family_cnt = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_cnt, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_cnt);
    vkGetPhysicalDeviceQueueFamilyProperties(device,
            &queue_family_cnt,
            queue_families.data());

    int i = 0;
    for (const auto &family : queue_families) {
        if (family.queueCount > 0 && family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = i;
            indices.graphics_family_has_value = true;
        }

        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->surface_, &present_support);

        if (family.queueCount > 0 && present_support) {
            indices.present_family = i;
            indices.present_family_has_value = true;
        }

        if (indices.is_complete())
            break;

        i++;
    }

    return indices;
}

swap_chain_support_details vk_device::query_swap_chain_support(VkPhysicalDevice device) {
    swap_chain_support_details details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device,
            this->surface_,
            &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device,
            this->surface_,
            &format_count,
            nullptr);

    if (format_count > 0) {
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device,
                this->surface_,
                &format_count, details.formats.data());
    }

    uint32_t present_mode_cnt;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device,
            this->surface_,
            &present_mode_cnt,
            nullptr);

    if (present_mode_cnt > 0) {
        details.present_modes.resize(present_mode_cnt);
        vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,
                this->surface_,
                &present_mode_cnt,
                details.present_modes.data());
    }

    return details;
}

VkFormat vk_device::find_supported_format(
        const std::vector<VkFormat> &candidates,
        VkImageTiling tiling,
        VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(this->physical_device_, format, &props);

        if ((tiling == VK_IMAGE_TILING_LINEAR &&
            (props.linearTilingFeatures & features) == features) ||
            (tiling == VK_IMAGE_TILING_OPTIMAL &&
            (props.optimalTilingFeatures & features) == features))
            return format;
    }

    throw vulkan_device_ex("failed to find supported device format!");
}

uint32_t vk_device::find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(this->physical_device_, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void vk_device::create_buffer(
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkMemoryPropertyFlags properties,
        VkBuffer &buffer,
        VkDeviceMemory &buffer_memory) {
    VkBufferCreateInfo buffer_info{};

    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(this->device_, &buffer_info, nullptr, &buffer) != VK_SUCCESS)
        throw vulkan_device_ex("failed to create buffer!");

    VkMemoryRequirements mem_reqs;
    vkGetBufferMemoryRequirements(this->device_, buffer, &mem_reqs);

    VkMemoryAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_reqs.size;
    alloc_info.memoryTypeIndex = find_memory_type(mem_reqs.memoryTypeBits, properties);

    if (vkAllocateMemory(this->device_, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS)
        throw vulkan_device_ex("failed to allocate buffer buffer_memory!");

    vkBindBufferMemory(this->device_, buffer, buffer_memory, 0);
}

VkCommandBuffer vk_device::begin_single_time_commands() {
    VkCommandBufferAllocateInfo alloc_info{};

    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = this->command_pool_;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(this->device_, &alloc_info, &command_buffer);

    VkCommandBufferBeginInfo begin_info{};

    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);
    return command_buffer;
}

void vk_device::end_single_time_commands(VkCommandBuffer command_buffer) {
    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info{};

    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    vkQueueSubmit(this->graphics_queue_, 1, &submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(this->graphics_queue_);

    vkFreeCommandBuffers(this->device_, this->command_pool_, 1, &command_buffer);
}

void vk_device::copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size) {
    VkCommandBuffer command_buffer = begin_single_time_commands();
    VkBufferCopy copy_region{};

    copy_region.srcOffset = 0;  // Optional
    copy_region.dstOffset = 0;  // Optional
    copy_region.size = size;

    vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1, &copy_region);

    end_single_time_commands(command_buffer);
}

void vk_device::copy_buffer_to_image(
        VkBuffer buffer,
        VkImage image,
        uint32_t width,
        uint32_t height,
        uint32_t layer_count) {
    VkCommandBuffer command_buffer = begin_single_time_commands();
    VkBufferImageCopy region{};

    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = layer_count;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1};

    vkCmdCopyBufferToImage(
            command_buffer,
            buffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region);
    end_single_time_commands(command_buffer);
}

void vk_device::create_image_with_info(
        const VkImageCreateInfo &image_info,
        VkMemoryPropertyFlags properties,
        VkImage &image,
        VkDeviceMemory &image_memory) {
    if (vkCreateImage(this->device_, &image_info, nullptr, &image) != VK_SUCCESS)
        throw vulkan_device_ex("failed to create image!");

    VkMemoryRequirements mem_reqs;
    vkGetImageMemoryRequirements(this->device_, image, &mem_reqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = mem_reqs.size;
    allocInfo.memoryTypeIndex = find_memory_type(mem_reqs.memoryTypeBits, properties);

    if (vkAllocateMemory(this->device_, &allocInfo, nullptr, &image_memory) != VK_SUCCESS)
        throw vulkan_device_ex("failed to allocate image memory!");

    if (vkBindImageMemory(this->device_, image, image_memory, 0) != VK_SUCCESS)
        throw vulkan_device_ex("failed to bind image memory!");
}

}