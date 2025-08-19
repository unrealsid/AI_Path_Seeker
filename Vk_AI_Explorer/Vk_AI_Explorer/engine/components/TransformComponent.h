#pragma once

#include "../Core/ECSTypes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../core/ECSTypes.h"
#include <vector>

namespace ecs::components
{
    struct TransformComponent
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat();
        glm::vec3 scale = glm::vec3(1.0f);
    
        // Cached matrices - updated by system
        glm::mat4 local_matrix = glm::mat4(1.0f);
        glm::mat4 world_matrix = glm::mat4(1.0f);
        glm::vec3 world_position = glm::vec3(0.0f);
    
        // Hierarchy data
        core::EntityID parent = core::INVALID_ENTITY;
        std::vector<core::EntityID> children;
    
        // Dirty flag for optimization
        bool dirty = true;
    
        // Constructor for easy initialization
        TransformComponent(const glm::vec3& pos = glm::vec3(0.0f), 
                          const glm::quat& rot = glm::quat(), 
                          const glm::vec3& scl = glm::vec3(1.0f))
            : position(pos), rotation(rot), scale(scl) {}
    };
}
