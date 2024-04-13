#pragma once

// huhu
#include "huhu_camera.hpp"
#include "huhu_pipeline.hpp"
#include "huhu_device.hpp"
#include "huhu_game_object.hpp"
#include "huhu_frame_info.hpp"

// std
#include <memory>
#include <vector>

namespace huhu
{
    class PointLightSystem
    {
    public:
        PointLightSystem(HuhuDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();

        PointLightSystem(const PointLightSystem &) = delete;
        PointLightSystem &operator=(const PointLightSystem &) = delete;

        void render(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        HuhuDevice &huhuDevice;

        std::unique_ptr<HuhuPipeline> huhuPipeline;
        VkPipelineLayout pipelineLayout;
    };
}