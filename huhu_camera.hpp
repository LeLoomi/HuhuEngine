#pragma once

// libs
#define GLM_FORCE_RADIANS           // make glm use radians over degrees on every OS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // make depth buffers range to 0 to 1
#include <glm/glm.hpp>

namespace huhu
{
    class HuhuCamera
    {
    public:
        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fovy, float aspect, float near, float far);
        const glm::mat4 &getProjection() const { return projectionMatrix; }

    private:
        glm::mat4 projectionMatrix{1.f};
    };
}