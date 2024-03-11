#pragma once

#include "huhu_camera.hpp"
#include "huhu_game_object.hpp"

// lib
#include <vulkan/vulkan.h>

namespace huhu
{
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