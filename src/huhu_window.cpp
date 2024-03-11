#include "huhu_window.hpp"

#include <stdexcept>
#include <string>

namespace huhu
{

    HuhuWindow::HuhuWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    HuhuWindow::~HuhuWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void HuhuWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void HuhuWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface");
        }
    }

    void HuhuWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto huhuWindow = reinterpret_cast<HuhuWindow *>(glfwGetWindowUserPointer(window));
        huhuWindow->framebufferResized = true;
        huhuWindow->width = width;
        huhuWindow->height = height;
    }
}