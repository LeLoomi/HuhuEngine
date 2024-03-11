#include "huhu_renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace huhu
{
    HuhuRenderer::HuhuRenderer(HuhuWindow &window, HuhuDevice &device) : huhuWindow{window}, huhuDevice{device}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    HuhuRenderer::~HuhuRenderer() { freeCommandBuffers(); }

    void HuhuRenderer::recreateSwapChain()
    {
        auto extent = huhuWindow.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = huhuWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(huhuDevice.device());

        if (huhuSwapChain == nullptr)
        {
            huhuSwapChain = std::make_unique<HuhuSwapChain>(huhuDevice, extent);
        }
        else
        {
            std::shared_ptr<HuhuSwapChain> oldSwapChain = std::move(huhuSwapChain);
            huhuSwapChain = std::make_unique<HuhuSwapChain>(huhuDevice, extent, oldSwapChain);

            if(!oldSwapChain->compareSwapFormats(*huhuSwapChain.get()))
            {
                throw std::runtime_error("Swap chain image or depth format has changed!");
            }
        }

        // we'll be back
    }

    void HuhuRenderer::createCommandBuffers()
    {
        commandBuffers.resize(HuhuSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = huhuDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(huhuDevice.device(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void HuhuRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            huhuDevice.device(),
            huhuDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer HuhuRenderer::beginFrame()
    {
        assert(!isFrameStarted && "Can't call beginFrame if a frame is already in progress!");

        auto result = huhuSwapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        return commandBuffer;
    }

    void HuhuRenderer::endFrame()
    {
        assert(isFrameStarted && "Can't call endFrame if no frame is in progress!");
    
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = huhuSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
            huhuWindow.wasWindowResized())
        {
            huhuWindow.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % HuhuSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void HuhuRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call beginSwapChainRenderPass if a frame is already in progress!");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't call beginSwapChainRenderPass with a command buffer from a different frame!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = huhuSwapChain->getRenderPass();
        renderPassInfo.framebuffer = huhuSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = huhuSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.001f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(huhuSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(huhuSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, huhuSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void HuhuRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call endSwapChainRenderPass if a frame is already in progress!");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't call endSwapChainRenderPass with a command buffer from a different frame!");

        vkCmdEndRenderPass(commandBuffer);
    }
}