#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <string>
namespace huhu
{

    class HuhuWindow
    {
    public:
        HuhuWindow(int w, int h, std::string name);
        ~HuhuWindow();

        HuhuWindow(const HuhuWindow &) = delete;
        HuhuWindow &operator=(const HuhuWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() { return framebufferResized; }
        void resetWindowResizedFlag() { framebufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        void initWindow();

        int width;
        int height;
        bool framebufferResized = false;

        std::string windowName;
        GLFWwindow *window;
    };
} // namespace lve