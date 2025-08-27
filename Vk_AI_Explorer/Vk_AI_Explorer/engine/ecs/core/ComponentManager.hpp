#pragma once

#include "ECSTypes.h"
#include "ComponentPool.h"
#include "../interfaces/IComponentPool.h"
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <cassert>

namespace ecs::core
{
    class ComponentManager
    {
    public:
        template<typename T>
        void register_component()
        {
            const char* type_name = typeid(T).name();

            assert(component_types.find(type_name) == component_types.end() && "Registering component type more than once.");

            // Assign the next available ID to this component type
            component_types[type_name] = next_component_type_id;
            // Create a new ComponentPool for this type
            component_pools[type_name] = std::make_unique<ComponentPool<T>>();
            next_component_type_id++;
        }

        // Retrieves the numeric ID for a given component type.
        template<typename T>
        ComponentTypeID get_component_type_id()
        {
            const char* type_name = typeid(T).name();
            assert(component_types.find(type_name) != component_types.end() && "Component not registered before use.");
            return component_types[type_name];
        }

        // Adds a component to an entity.
        template<typename T, typename... Args>
        T& add_component(EntityID entity, Args&&... args)
        {
            return get_component_pool<T>()->add_component(entity, T{std::forward<Args>(args)...});
        }

        // Removes a component from an entity.
        template<typename T>
        void remove_component(EntityID entity)
        {
            get_component_pool<T>()->remove_component(entity);
        }

        // Retrieves a component for a given entity.
        template<typename T>
        T& get_component(EntityID entity)
        {
            return get_component_pool<T>()->get_component(entity);
        }
        
        // Checks if an entity has a specific component.
        template<typename T>
        bool has_component(EntityID entity)
        {
            return get_component_pool<T>()->has_component(entity);
        }

        // Called when an entity is destroyed to clean up all its components.
        void entity_destroyed(EntityID entity)
        {
            // Iterate through all component pools and remove the entity if it exists
            for (auto const& pair : component_pools)
            {
                auto const& pool = pair.second;
                pool->remove_component(entity);
            }
        }

    private:
        // Retrieves the ComponentPool for a specific component type.
        template<typename T>
        ComponentPool<T>* get_component_pool()
        {
            const char* type_name = typeid(T).name();
            assert(component_pools.find(type_name) != component_pools.end() && "Component not registered before use.");
            return static_cast<ComponentPool<T>*>(component_pools[type_name].get());
        }

        // Map from component type name (string) to its unique numeric ID
        std::unordered_map<const char*, ComponentTypeID> component_types{};
        // Map from component type name (string) to its data pool
        std::unordered_map<const char*, std::unique_ptr<ecs::interfaces::IComponentPool>> component_pools{};
        // The next available component ID to be assigned
        ComponentTypeID next_component_type_id = 0;
    };
}
