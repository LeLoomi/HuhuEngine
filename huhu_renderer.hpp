#pragma once

// huhu
#include "huhu_window.hpp"
#include "huhu_device.hpp"
#include "huhu_swap_chain.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>

namespace huhu
{
    class HuhuRenderer
    {
    public:
        HuhuRenderer(HuhuWindow &window, HuhuDevice &device);
        ~HuhuRenderer();

        HuhuRenderer(const HuhuRenderer &) = delete;
        HuhuRenderer &operator=(const HuhuRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return huhuSwapChain->getRenderPass(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame is not in progress!");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index buffer when frame is not in progress!");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        HuhuWindow &huhuWindow;
        HuhuDevice &huhuDevice;
        std::unique_ptr<HuhuSwapChain> huhuSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex = 0;
        bool isFrameStarted{false};
    };
}