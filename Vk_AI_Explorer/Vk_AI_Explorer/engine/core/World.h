#pragma once

#include "ECSTypes.h"
#include "EntityManager.h"
#include "ComponentPool.h"
#include "ComponentRegistry.h"
#include <memory>
#include <unordered_map>

namespace ecs::core
{
    class World
    {
    public:
        World() = default;
        ~World() = default;

        EntityID create_entity()
        {
            return entity_manager.create_entity();
        }

        EntityID create_entity(const std::string& tag)
        {
            return entity_manager.create_entity(tag);
        }

        void destroy_entity(EntityID entity)
        {
            // Remove all components
            for (auto& pool : component_pools)
            {
                if (pool.second->has_component(entity))
                {
                    pool.second->remove_component(entity);
                }
            }
            
            entity_manager.destroy_entity(entity);
        }

        template<typename T, typename... Args>
        T& add_component(EntityID entity, Args&&... args)
        {
            ComponentTypeID type_id = ComponentRegistry::get_component_type_id<T>();
            
            if (component_pools.find(type_id) == component_pools.end())
            {
                component_pools[type_id] = std::make_unique<ComponentPool<T>>();
            }
            
            auto pool = static_cast<ComponentPool<T>*>(component_pools[type_id].get());
            T& component = pool->add_component(entity, T{std::forward<Args>(args)...});
            
            entity_manager.get_component_mask(entity).set(type_id);
            
            return component;
        }

        template<typename T>
        T& get_component(EntityID entity)
        {
            ComponentTypeID type_id = ComponentRegistry::get_component_type_id<T>();
            auto pool = static_cast<ComponentPool<T>*>(component_pools[type_id].get());
            return pool->get_component(entity);
        }

        template<typename T>
        const T& get_component(EntityID entity) const
        {
            ComponentTypeID type_id = ComponentRegistry::get_component_type_id<T>();
            auto pool = static_cast<ComponentPool<T>*>(component_pools.at(type_id).get());
            return pool->get_component(entity);
        }

        template<typename T>
        bool has_component(EntityID entity) const
        {
            ComponentTypeID type_id = ComponentRegistry::get_component_type_id<T>();
            auto it = component_pools.find(type_id);
            return it != component_pools.end() && it->second->has_component(entity);
        }

        template<typename T>
        void remove_component(EntityID entity)
        {
            ComponentTypeID type_id = ComponentRegistry::get_component_type_id<T>();
            auto it = component_pools.find(type_id);
            if (it != component_pools.end())
            {
                it->second->remove_component(entity);
                entity_manager.get_component_mask(entity).reset(type_id);
            }
        }

        template<typename T>
        ComponentPool<T>* get_component_pool()
        {
            ComponentTypeID type_id = ComponentRegistry::get_component_type_id<T>();
            auto it = component_pools.find(type_id);
            return (it != component_pools.end()) ? 
                   static_cast<ComponentPool<T>*>(it->second.get()) : nullptr;
        }

        EntityID get_entity_by_tag(const std::string& tag) const
        {
            return entity_manager.get_entity_by_tag(tag);
        }

        const std::string& get_entity_tag(EntityID entity) const
        {
            return entity_manager.get_entity_tag(entity);
        }

        template<typename... Components>
        std::vector<EntityID> query_entities() const
        {
            ComponentMask mask;
            (mask.set(ComponentRegistry::get_component_type_id<Components>()), ...);
            return entity_manager.query_entities(mask);
        }

        std::vector<EntityID> query_entities(const ComponentMask& mask) const
        {
            return entity_manager.query_entities(mask);
        }

    private:
        EntityManager entity_manager;
        std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentPool>> component_pools;
    };
}
