#pragma once

#include "huhu_device.hpp"

// std
#include <string>
#include <vector>

namespace huhu
{
    struct PipelineConfigInfo
    {
    };

    class HuhuPipeline
    {
    public:
        HuhuPipeline(HuhuDevice &device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
        ~HuhuPipeline(){};

        HuhuPipeline(const HuhuPipeline &) = delete;
        void operator=(const HuhuPipeline &) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filepath);

        void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        HuhuDevice &huhuDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}