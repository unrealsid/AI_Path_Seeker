#pragma once
#include "../interfaces/ISystem.h"
#include "../core/World.hpp"
#include "../components/TransformComponent.h"
#include "../components/VelocityComponent.h"

namespace ecs::system
{
    class MovementSystem : public interface::ISystem
    {
    public:
        void update(core::World& world, double delta_time) override
        {
            // Iterate over all entities that have the required signature
            for (auto const& entity : entities)
            {
                // Retrieve the Transform and Velocity components for the entity
                auto& transform = world.get_component<component::TransformComponent>(entity);
                auto& velocity = world.get_component<component::VelocityComponent>(entity);

                // Update the position
                transform.position += velocity.value * (float) delta_time;
            }
        }
    };
}