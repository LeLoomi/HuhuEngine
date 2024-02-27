#pragma once

#include "huhu_model.hpp"

// std
#include <memory>

namespace huhu
{
    struct Transform2dComponent
    {
        glm::vec2 translation{}; // (position offset)

        glm::mat2 mat2() { return glm::mat2{1.f}; };
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