#pragma once

#include "huhu_window.hpp"
#include "huhu_pipeline.hpp"
#include "huhu_device.hpp"

namespace huhu
{
    class FirstApp
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

    private:
        HuhuWindow huhuWindow{WIDTH, HEIGHT, "Hoot hoot!"};
        HuhuDevice huhuDevice{huhuWindow};
        HuhuPipeline huhuPipeline{huhuDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", HuhuPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}