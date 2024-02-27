#pragma once

#include "huhu_model.hpp"

// std
#include <memory>

namespace huhu
{
    struct Transform2dComponent
    {
        glm::vec2 translation{}; // (position offset)
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2()
        {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMat{{c, s}, {-s, c}};   // see https://mathworld.wolfram.com/RotationMatrix.html

            glm::mat2 scaleMat = {{scale.x, 0.f}, {0.f, scale.y}}; // mat2 takes matrix as columns! { {col1}, {col2} }
            return rotMat * scaleMat;
        };
    };

    class HuhuGameObject
    {
    public:
        using id_t = unsigned int;

        static HuhuGameObject createGameObject()
        {
            static id_t currentId = 0;
            return HuhuGameObject(currentId++);
        }

        HuhuGameObject(const HuhuGameObject &) = delete;
        HuhuGameObject &operator=(const HuhuGameObject &) = delete;
        HuhuGameObject(HuhuGameObject &&) = default;
        HuhuGameObject &operator=(HuhuGameObject &&) = default;

        const id_t getId() { return id; };

        std::shared_ptr<HuhuModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
        HuhuGameObject(id_t objId) : id{objId} {};

        id_t id;
    };
}