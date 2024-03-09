#pragma once

#include "huhu_camera.hpp"

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
    };
}