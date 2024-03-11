#pragma once

// huhu
#include "huhu_window.hpp"
#include "huhu_device.hpp"
#include "huhu_game_object.hpp"
#include "huhu_renderer.hpp"
#include "huhu_descriptors.hpp"

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
        void loadGameObjects();

        HuhuWindow huhuWindow{WIDTH, HEIGHT, "Hoot hoot!"};
        HuhuDevice huhuDevice{huhuWindow};
        HuhuRenderer huhuRenderer{huhuWindow, huhuDevice};

        std::unique_ptr<HuhuDescriptorPool> globalPool{};
        HuhuGameObject::Map gameObjects;
    };
}