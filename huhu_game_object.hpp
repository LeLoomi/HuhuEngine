#pragma once

#include "huhu_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace huhu
{
    struct TransformComponent
    {
        glm::vec3 translation{}; // (position offset)
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation;

        // Matrix corresponds to translate * Ry * Rx * Rz * scale transform
        // Rotation order uses Tait-Bryan angles with axis order Y(1), X(2), Y(3)
        glm::mat4 mat4()
        {
            auto transform = glm::translate(glm::mat4{1.f}, translation);
            transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
            transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
            transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});
            transform = glm::scale(transform, scale);
            return transform;
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
        TransformComponent transform{};

    private:
        HuhuGameObject(id_t objId) : id{objId} {};

        id_t id;
    };
}