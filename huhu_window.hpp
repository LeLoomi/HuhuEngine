#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <string>

namespace huhu {

    class HuhuWindow {

        public:
        HuhuWindow(int w, int h, std::string name);
        ~HuhuWindow();

        HuhuWindow(const HuhuWindow &) = delete;
        HuhuWindow &operator = (const HuhuWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; };

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        private:
        void initWindow();
        
        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;
    };
}