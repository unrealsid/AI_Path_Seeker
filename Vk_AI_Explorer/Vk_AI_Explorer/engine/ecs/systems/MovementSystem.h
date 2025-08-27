#pragma once
#include "engine/ecs/interfaces/ISystem.h"
#include "engine/ecs/core/World.hpp"
#include "engine/ecs/components/TransformComponent.h"
#include "engine/ecs/components/VelocityComponent.h"

namespace ecs::systems
{
    class MovementSystem : public interfaces::ISystem
    {
    public:
        void update(core::World& world, double delta_time) override
        {
            // Iterate over all entities that have the required signature
            for (auto const& entity : entities)
            {
                // Retrieve the Transform and Velocity components for the entity
                auto& transform = world.get_component<components::TransformComponent>(entity);
                auto& velocity = world.get_component<components::VelocityComponent>(entity);

                // Update the position
                transform.position += velocity.value * (float) delta_time;
            }
        }
    };
}