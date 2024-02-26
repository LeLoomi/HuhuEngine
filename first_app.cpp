#include "first_app.hpp"

// libs
#define GLM_FORCE_RADIANS           // make glm use radians over degrees on every OS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // make depth buffers range to 0 to 1
#include <glm/glm.hpp>

// std
#include <stdexcept>
#include <array>
#include <cassert>

namespace huhu
{
    // implemented in cpp only since we will eventually move away from it
    struct SimplePushConstantData
    {
        glm::vec2 offset;
        glm::vec3 color;
    };

    FirstApp::FirstApp()
    {
        loadModels();
        createPipelineLayout();
        recreateSwapChain(); // this includes creating the first swap chain
        createCommandBuffers();
    }

    FirstApp::~FirstApp()
    {
        vkDestroyPipelineLayout(huhuDevice.device(), pipelineLayout, nullptr);
    }

    void FirstApp::run()
    {
        while (!huhuWindow.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(huhuDevice.device()); // have cpu wait for gpu to finish before we shut down
    }

    void FirstApp::loadModels()
    {
        std::vector<HuhuModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

        huhuModel = std::make_unique<HuhuModel>(huhuDevice, vertices);
    }

    void FirstApp::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange;
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(huhuDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline()
    {
        assert(huhuSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        HuhuPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = huhuSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        huhuPipeline = std::make_unique<HuhuPipeline>(
            huhuDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void FirstApp::recreateSwapChain()
    {
        auto extend = huhuWindow.getExtent();
        while (extend.width == 0 || extend.height == 0) // to handle e.g. minimization of the window
        {
            extend = huhuWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(huhuDevice.device()); // wait until old swapchain is no longer in use before creating a new one

        if (huhuSwapChain == nullptr)
        {
            huhuSwapChain = std::make_unique<HuhuSwapChain>(huhuDevice, extend);
        }
        else
        {
            huhuSwapChain = std::make_unique<HuhuSwapChain>(huhuDevice, extend, std::move(huhuSwapChain));
            if (huhuSwapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        // Future: if render pass is compatible do nothing else:
        createPipeline();
    }

    void FirstApp::createCommandBuffers()
    {
        commandBuffers.resize(huhuSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = huhuDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(huhuDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void FirstApp::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            huhuDevice.device(),
            huhuDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data());
        commandBuffers.clear();
    }

    void FirstApp::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = huhuSwapChain->getRenderPass();
        renderPassInfo.framebuffer = huhuSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = huhuSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(huhuSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(huhuSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, huhuSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        huhuPipeline->bind(commandBuffers[imageIndex]);
        huhuModel->bind(commandBuffers[imageIndex]);

        for (int j = 0; j < 4; j++)
        {
            SimplePushConstantData push{};
            push.offset = {0.0f, -0.4f + j * 0.25f};
            push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

            vkCmdPushConstants(
                commandBuffers[j],
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);
            huhuModel->draw(commandBuffers[imageIndex]);
        }

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void FirstApp::drawFrame()
    {
        uint32_t imageIndex;
        auto result = huhuSwapChain->acquireNextImage(&imageIndex);

        if (result != VK_ERROR_OUT_OF_DATE_KHR) // in case resizing has made verticies wierd? see spec for the error
        {
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            std::runtime_error("failed to aquire next swapchain image!"); // handling this properly is needed to support rezisable windows!
        }

        recordCommandBuffer(imageIndex);
        result = huhuSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || huhuWindow.wasWindowResized())
        {
            huhuWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS)
        {
            std::runtime_error("failed to present swap chain image!");
        }
    }
}