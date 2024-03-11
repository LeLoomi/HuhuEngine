#pragma once

#include "huhu_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace huhu
{
    class HuhuDescriptorSetLayout
    {
    public:
        class Builder
        {
        public:
            Builder(HuhuDevice &huhuDevice) : huhuDevice{huhuDevice} {}

            Builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<HuhuDescriptorSetLayout> build() const;

        private:
            HuhuDevice &huhuDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        HuhuDescriptorSetLayout(
            HuhuDevice &huhuDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~HuhuDescriptorSetLayout();
        HuhuDescriptorSetLayout(const HuhuDescriptorSetLayout &) = delete;
        HuhuDescriptorSetLayout &operator=(const HuhuDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        HuhuDevice &huhuDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class HuhuDescriptorWriter;
    };

    class HuhuDescriptorPool
    {
    public:
        class Builder
        {
        public:
            Builder(HuhuDevice &huhuDevice) : huhuDevice{huhuDevice} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<HuhuDescriptorPool> build() const;

        private:
            HuhuDevice &huhuDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        HuhuDescriptorPool(
            HuhuDevice &huhuDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~HuhuDescriptorPool();
        HuhuDescriptorPool(const HuhuDescriptorPool &) = delete;
        HuhuDescriptorPool &operator=(const HuhuDescriptorPool &) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

    private:
        HuhuDevice &huhuDevice;
        VkDescriptorPool descriptorPool;

        friend class HuhuDescriptorWriter;
    };

    class HuhuDescriptorWriter
    {
    public:
        HuhuDescriptorWriter(HuhuDescriptorSetLayout &setLayout, HuhuDescriptorPool &pool);

        HuhuDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        HuhuDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        HuhuDescriptorSetLayout &setLayout;
        HuhuDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}