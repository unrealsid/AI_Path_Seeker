#pragma once

#include "ECSTypes.h"
#include "../interfaces/IComponentPool.h"
#include <vector>
#include <unordered_map>
#include <memory>

namespace ecs::core
{
    // A data structure that stores all components of a single type.
    // It uses a packed array for cache efficiency.
    template<typename T>
    class ComponentPool : public interface::IComponentPool
    {
    public:
        // Adds a component for a given entity.
        T& add_component(EntityID entity, T&& component)
        {
            // Ensure we don't add a component to the same entity twice
            if (entity_to_index.contains(entity))
            {
                return get_component(entity);
            }

            // Add the new component to the end of the packed array
            size_t new_index = components.size();
            entity_to_index[entity] = new_index;
            index_to_entity.push_back(entity);
            components.emplace_back(std::forward<T>(component));
            return components.back();
        }

        // Retrieves the component for a given entity.
        T& get_component(EntityID entity)
        {
            return components[entity_to_index.at(entity)];
        }

        // Removes a component from an entity using the swap-and-pop method.
        // This is very fast (O(1)) but changes the order of components in the array.
        void remove_component(EntityID entity) override
        {
            // If the entity doesn't have this component, do nothing.
            if (entity_to_index.find(entity) == entity_to_index.end())
            {
                return;
            }

            // Get the index of the component to remove
            size_t index_to_remove = entity_to_index[entity];
            size_t last_index = components.size() - 1;

            // Move the last element into the place of the one being removed
            components[index_to_remove] = std::move(components[last_index]);

            // Update the mapping for the moved element
            EntityID last_entity = index_to_entity[last_index];
            entity_to_index[last_entity] = index_to_remove;
            index_to_entity[index_to_remove] = last_entity;

            // Erase the now-duplicate last element
            components.pop_back();
            index_to_entity.pop_back();
            entity_to_index.erase(entity);
        }

        // Checks if an entity has a component in this pool.
        bool has_component(EntityID entity) const
        {
            return entity_to_index.contains(entity);
        }

    private:
        // The packed array of components.
        std::vector<T> components;
        // Map from an entity ID to its index in the components array.
        std::unordered_map<EntityID, size_t> entity_to_index;
        // Map from an index in the array back to an entity ID.
        std::vector<EntityID> index_to_entity;
    };
}
