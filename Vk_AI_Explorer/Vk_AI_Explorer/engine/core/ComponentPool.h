#pragma once

#include "ECSTypes.h"
#include <vector>
#include <unordered_map>
#include <memory>
#include "../interfaces/IComponentPool.h"

namespace ecs::core
{
    template<typename T>
    class ComponentPool : public IComponentPool
    {
    public:
        T& add_component(EntityID entity, T&& component)
        {
            entity_to_index[entity] = components.size();
            index_to_entity.push_back(entity);
            components.emplace_back(std::forward<T>(component));
            return components.back();
        }

        T& get_component(EntityID entity)
        {
            return components[entity_to_index[entity]];
        }

        void remove_component(EntityID entity) override
        {
            auto index_to_remove = entity_to_index[entity];
            auto last_index = components.size() - 1;
            
            // Swap with last element
            components[index_to_remove] = std::move(components[last_index]);
            
            // Update mappings
            auto last_entity = index_to_entity[last_index];
            entity_to_index[last_entity] = index_to_remove;
            index_to_entity[index_to_remove] = last_entity;
            
            // Remove last element
            components.pop_back();
            index_to_entity.pop_back();
            entity_to_index.erase(entity);
        }

        bool has_component(EntityID entity) const override
        {
            return entity_to_index.find(entity) != entity_to_index.end();
        }

        std::vector<T>& get_components() { return components; }
        const std::vector<T>& get_components() const { return components; }
        const std::vector<EntityID>& get_entities() const { return index_to_entity; }

    private:
        std::vector<T> components;
        std::unordered_map<EntityID, size_t> entity_to_index;
        std::vector<EntityID> index_to_entity;
    };
}
