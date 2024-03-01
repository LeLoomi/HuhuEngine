#include "first_app.hpp"

#include "huhu_camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>

namespace huhu
{
    FirstApp::FirstApp()
    {
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run()
    {
        SimpleRenderSystem simpleRenderSystem{huhuDevice, huhuRenderer.getSwapChainRenderPass()};
        HuhuCamera camera{};

        auto viewerObject = HuhuGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!huhuWindow.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneYXZ(huhuWindow.getGlfwWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = huhuRenderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

            if (auto commandBuffer = huhuRenderer.beginFrame())
            {
                huhuRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                huhuRenderer.endSwapChainRenderPass(commandBuffer);
                huhuRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(huhuDevice.device());
    }

    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<HuhuModel> huhuModel =
            HuhuModel::createModelFromFile(huhuDevice, "models/smooth_vase.obj");
        auto gameObj = HuhuGameObject::createGameObject();
        gameObj.model = huhuModel;
        gameObj.transform.translation = {.0f, .0f, 2.5f};
        gameObj.transform.scale = {.5f, .5f, .5f};

        gameObjects.push_back(std::move(gameObj));
    }
}