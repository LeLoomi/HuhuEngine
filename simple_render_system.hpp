#pragma once

// huhu
#include "huhu_pipeline.hpp"
#include "huhu_device.hpp"
#include "huhu_game_object.hpp"

// std
#include <memory>
#include <vector>

namespace huhu
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(HuhuDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<HuhuGameObject> &gameObjects);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        HuhuDevice &huhuDevice;

        std::unique_ptr<HuhuPipeline> huhuPipeline;
        VkPipelineLayout pipelineLayout;
    };
}