#pragma once

#include "../Core/ECSTypes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../core/ECSTypes.h"

namespace ecs::components
{
    struct Transform
    {
        glm::vec3 position{0.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f};
        ecs::core::EntityID parent = ecs::core::INVALID_ENTITY;

        glm::mat4 get_local_matrix() const
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 rotation_matrix = glm::mat4_cast(rotation);
            glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
            return translation * rotation_matrix * scale_matrix;
        }

        void set_rotation(const glm::vec3& euler_angles)
        {
            rotation = glm::quat(glm::radians(euler_angles));
        }

        glm::vec3 get_rotation_euler() const
        {
            return glm::degrees(glm::eulerAngles(rotation));
        }
    };
}
