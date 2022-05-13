//
// Created by Garrett on 3/30/22.
//

#include "vk_descriptor.hpp"

#include <cassert>
#include <stdexcept>

namespace glw {

// *************** Descriptor Set Layout builder *********************

vk_descriptor_set_layout::builder &vk_descriptor_set_layout::builder::add_binding(
        uint32_t binding,
        VkDescriptorType descriptor_type,
        VkShaderStageFlags stage_flags,
        uint32_t count) {
    assert(this->bindings_.count(binding) == 0 && "Binding already in use");

    VkDescriptorSetLayoutBinding layoutBinding{};

    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptor_type;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stage_flags;

    this->bindings_[binding] = layoutBinding;

    return *this;
}

vk_descriptor_set_layout *vk_descriptor_set_layout::builder::build() const {
    return new vk_descriptor_set_layout(this->device_, this->bindings_);
}

// *************** Descriptor Set Layout *********************

vk_descriptor_set_layout::vk_descriptor_set_layout(vk_device *d,
        const std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding>& bindings)
        : device_{d}, bindings_{bindings} {
    std::vector<VkDescriptorSetLayoutBinding> set_layout_bindings{};

    for (auto [k, v] : bindings) {
        set_layout_bindings.push_back(v);
    }

    VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info{};
    descriptor_set_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_set_layout_info.bindingCount = static_cast<uint32_t>(set_layout_bindings.size());
    descriptor_set_layout_info.pBindings = set_layout_bindings.data();

    if (vkCreateDescriptorSetLayout(
            this->device_->device_,
            &descriptor_set_layout_info,
            nullptr,
            &descriptor_set_layout_) != VK_SUCCESS)
        throw std::runtime_error("failed to create descriptor set layout!");
}

vk_descriptor_set_layout::~vk_descriptor_set_layout() {
    vkDestroyDescriptorSetLayout(this->device_->device_,
            this->descriptor_set_layout_,
            nullptr);
}

// *************** Descriptor Pool builder *********************

vk_descriptor_pool::builder &vk_descriptor_pool::builder::add_pool_size(
        VkDescriptorType descriptor_type, uint32_t count) {
    this->pool_sizes_.push_back({descriptor_type, count});
    return *this;
}

vk_descriptor_pool::builder &vk_descriptor_pool::builder::set_pool_flags(
        VkDescriptorPoolCreateFlags flags) {
    this->pool_flags_ = flags;
    return *this;
}
vk_descriptor_pool::builder &vk_descriptor_pool::builder::set_max_sets(uint32_t count) {
    this->max_sets_ = count;
    return *this;
}

vk_descriptor_pool *vk_descriptor_pool::builder::build() const {
    return new vk_descriptor_pool(this->device_,
            this->max_sets_,
            this->pool_flags_,
            this->pool_sizes_);
}

// *************** Descriptor Pool *********************

vk_descriptor_pool::vk_descriptor_pool(
        vk_device *d,
        uint32_t maxSets,
        VkDescriptorPoolCreateFlags poolFlags,
        const std::vector<VkDescriptorPoolSize> &poolSizes)
        : device_{d} {
    VkDescriptorPoolCreateInfo descriptor_pool_info{};

    descriptor_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_info.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptor_pool_info.pPoolSizes = poolSizes.data();
    descriptor_pool_info.maxSets = maxSets;
    descriptor_pool_info.flags = poolFlags;

    if (vkCreateDescriptorPool(this->device_->device_,
            &descriptor_pool_info,
            nullptr,
            &descriptor_pool_) != VK_SUCCESS)
        throw std::runtime_error("failed to create descriptor pool_!");
}

vk_descriptor_pool::~vk_descriptor_pool() {
    vkDestroyDescriptorPool(this->device_->device_, this->descriptor_pool_, nullptr);
}

bool vk_descriptor_pool::allocate_descriptor_set(
        VkDescriptorSetLayout descriptor_set_layout,
        VkDescriptorSet &descriptor) const {
    VkDescriptorSetAllocateInfo alloc_info{};

    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = descriptor_pool_;
    alloc_info.pSetLayouts = &descriptor_set_layout;
    alloc_info.descriptorSetCount = 1;

    // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
    // a new pool_ whenever an old pool_ fills up. But this is beyond our current scope
    return vkAllocateDescriptorSets(this->device_->device_,
            &alloc_info,
            &descriptor) == VK_SUCCESS;
}

void vk_descriptor_pool::free_descriptors(std::vector<VkDescriptorSet> &descriptors) const {
    vkFreeDescriptorSets(
            this->device_->device_,
            this->descriptor_pool_,
            static_cast<uint32_t>(descriptors.size()),
            descriptors.data());
}

void vk_descriptor_pool::reset_pool() {
    vkResetDescriptorPool(this->device_->device_, this->descriptor_pool_, 0);
}

// *************** Descriptor Writer *********************

vk_descriptor_writer::vk_descriptor_writer(vk_descriptor_set_layout &set_layout,
        vk_descriptor_pool &pool)
        : set_layout_{set_layout}, pool_{pool}
{

}

vk_descriptor_writer &vk_descriptor_writer::write_buffer(
        uint32_t binding, VkDescriptorBufferInfo *buffer_info) {
    assert(this->set_layout_.bindings_.count(binding) == 1 &&
        "Layout does not contain specified binding");

    auto &binding_description = this->set_layout_.bindings_[binding];

    assert(binding_description.descriptorCount == 1 &&
                    "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};

    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = binding_description.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = buffer_info;
    write.descriptorCount = 1;

    this->writes_.push_back(write);
    return *this;
}

vk_descriptor_writer &vk_descriptor_writer::write_image(
        uint32_t binding, VkDescriptorImageInfo *image_info) {
    assert(this->set_layout_.bindings_.count(binding) == 1 &&
        "Layout does not contain specified binding");

    auto &binding_description = this->set_layout_.bindings_[binding];

    assert(binding_description.descriptorCount == 1 &&
                    "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};

    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = binding_description.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = image_info;
    write.descriptorCount = 1;

    this->writes_.push_back(write);
    return *this;
}

bool vk_descriptor_writer::build(VkDescriptorSet &set) {
    if (!this->pool_.allocate_descriptor_set(
            this->set_layout_.get_descriptor_set_layout(),
            set))
        return false;

    overwrite(set);
    return true;
}

void vk_descriptor_writer::overwrite(VkDescriptorSet &set) {
    for (auto &write : this->writes_) {
        write.dstSet = set;
    }

    vkUpdateDescriptorSets(this->pool_.device_->device_,
            this->writes_.size(),
            this->writes_.data(),
            0,
            nullptr);
}

}