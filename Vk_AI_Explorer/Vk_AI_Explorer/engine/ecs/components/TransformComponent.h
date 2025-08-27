#pragma once

#include "glm/glm.hpp"

namespace ecs::component
{
    struct TransformComponent
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };
}