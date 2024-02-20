#include "first_app.hpp"

// std
#include <stdexcept>

namespace huhu
{
    FirstApp::FirstApp()
    {
        createPipelineLayout();
        createPipeline();
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
        }
    }

    void FirstApp::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo;
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if (vkCreatePipelineLayout(huhuDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline()
    {
        auto pipelineConfig = HuhuPipeline::defaultPipelineConfigInfo(huhuSwapChain.width(), huhuSwapChain.height());
        pipelineConfig.renderPass = huhuSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        huhuPipeline = std::make_unique<HuhuPipeline>(
            huhuDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig);
    }

    void FirstApp::createCommandBuffers() {}
    void FirstApp::drawFrame() {}
}