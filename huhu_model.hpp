#pragma once

#include "huhu_device.hpp"

// libs
#define GLM_FORCE_RADIANS           // make glm use radians over degrees on every OS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // make depth buffers range to 0 to 1
#include <glm/glm.hpp>

// std
#include <vector>

namespace huhu
{
    class HuhuModel
    {
    public:
        struct Vertex
        {
            glm::vec2 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        HuhuModel(HuhuDevice &device, const std::vector<Vertex> &vertices);
        ~HuhuModel();

        HuhuModel(const HuhuModel &) = delete;
        HuhuModel &operator=(const HuhuModel &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        HuhuDevice &huhuDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}