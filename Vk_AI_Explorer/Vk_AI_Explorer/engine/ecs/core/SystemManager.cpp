#include "SystemManager.h"
#include "../Interfaces/ISystem.h"

void ecs::core::SystemManager::entity_signature_changed(EntityID entity, Signature entity_signature)
{
    for (auto const& [type, system] : systems)
    {
        auto const& system_signature = system_signatures[type];

        // If entity signature matches system signature
        if ((entity_signature & system_signature) == system_signature)
        {
            // Insert the entity into the system's set.
            // std::set handles duplicates automatically.
            system->entities.insert(entity);
        }
        // If it doesn't match
        else
        {
            // Remove the entity from the system's set.
            system->entities.erase(entity);
        }
    }
}

void ecs::core::SystemManager::entity_destroyed(EntityID entity)
{
    for (auto const& [name, system] : systems)
    {
        system->entities.erase(entity);
    }
}

void ecs::core::SystemManager::update_all_systems(World& world, double delta_time)
{
    for (auto& [name, system] : systems)
    {
        system->update(world, delta_time);
    }
}
