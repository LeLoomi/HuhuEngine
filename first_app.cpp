#include "first_app.hpp"

#include "huhu_camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"
#include "huhu_buffer.hpp"

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
    struct GlobalUbo
    {
        alignas(16) glm::mat4 projectionView{1.f};
        alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };

    FirstApp::FirstApp()
    {
        globalPool = HuhuDescriptorPool::Builder(huhuDevice)
                         .setMaxSets(HuhuSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, HuhuSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .build();
        loadGameObjects();
    }

    FirstApp::~FirstApp() {}

    void FirstApp::run()
    {
        std::vector<std::unique_ptr<HuhuBuffer>> uboBuffers(HuhuSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<HuhuBuffer>(
                huhuDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = HuhuDescriptorSetLayout::Builder(huhuDevice)
                                   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
                                   .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(HuhuSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            HuhuDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{
            huhuDevice,
            huhuRenderer.getSwapChainRenderPass(),
            globalSetLayout->getDescriptorSetLayout()};
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
                int frameIndex = huhuRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex]};

                // updating
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // rendering
                huhuRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
                huhuRenderer.endSwapChainRenderPass(commandBuffer);
                huhuRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(huhuDevice.device());
    }

    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<HuhuModel> huhuModel =
            HuhuModel::createModelFromFile(huhuDevice, "models/flat_vase.obj");
        auto gameObj = HuhuGameObject::createGameObject();
        gameObj.model = huhuModel;
        gameObj.transform.translation = {.0f, 0.5f, 2.5f};
        gameObj.transform.scale = {2.5f, 2.5f, 2.5f};

        gameObjects.push_back(std::move(gameObj));
    }
}