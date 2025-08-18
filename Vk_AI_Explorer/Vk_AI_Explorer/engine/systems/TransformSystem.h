#pragma once

#include "../interfaces/ISystem.h"
#include "../Components/Transform.h"
#include "../structs/EngineContext.h"
#include <glm/glm.hpp>

namespace ecs::systems
{
    class TransformSystem : public ecs::core::ISystem
    {
    public:
        TransformSystem(EngineContext& engine_context) : engine_context(engine_context) {}

        void update(ecs::core::World& world, double delta_time) override;

    private:
        EngineContext& engine_context;

        glm::mat4 calculate_world_matrix(ecs::core::World& world, ecs::core::EntityID entity, 
                                        const components::Transform& transform);
    };
}
