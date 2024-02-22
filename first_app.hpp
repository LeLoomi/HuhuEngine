#pragma once

// huhu
#include "huhu_window.hpp"
#include "huhu_pipeline.hpp"
#include "huhu_device.hpp"
#include "huhu_swap_chain.hpp"
#include "huhu_model.hpp"

// std
#include <memory>
#include <vector>

namespace huhu
{
    class FirstApp
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;

        void run();

    private:
    void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);

        HuhuWindow huhuWindow{WIDTH, HEIGHT, "Hoot hoot!"};
        HuhuDevice huhuDevice{huhuWindow};
        std::unique_ptr<HuhuSwapChain> huhuSwapChain;
        std::unique_ptr<HuhuPipeline> huhuPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<HuhuModel> huhuModel;
    };
}