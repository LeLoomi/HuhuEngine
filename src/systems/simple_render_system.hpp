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
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(HuhuDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        HuhuDevice &huhuDevice;

        std::unique_ptr<HuhuPipeline> huhuPipeline;
        VkPipelineLayout pipelineLayout;
    };
}