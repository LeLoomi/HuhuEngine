#pragma once

#include "huhu_camera.hpp"
#include "huhu_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace huhu
{
    #define MAX_LIGHTS 10

    struct PointLight {
        glm::vec4 position{};   // ignore w
        glm::vec4 color{};      // w is intesity
    };

    struct GlobalUbo
    {
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        // glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
        glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        HuhuCamera &camera;
        VkDescriptorSet globalDescriptorSet;
        HuhuGameObject::Map &gameObjects;
    };
}