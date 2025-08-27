#pragma once

#include "ECSTypes.h"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.h"
#include <memory>

namespace ecs::core
{
    class World
    {
    public:
        World()
        {
            // Allocate the managers on creation
            entity_manager = std::make_unique<EntityManager>();
            component_manager = std::make_unique<ComponentManager>();
            system_manager = std::make_unique<SystemManager>();
        }
        
        // --- Entity Methods ---
        EntityID create_entity() const
        {
            return entity_manager->create_entity();
        }

        EntityID create_entity(const std::string& name ) const
        {
            return entity_manager->create_entity(name);
        }

        void destroy_entity(EntityID entity) const
        {
            entity_manager->destroy_entity(entity);
            component_manager->entity_destroyed(entity);
            system_manager->entity_destroyed(entity);
        }

        // --- Component Methods ---
        template<typename T>
        void register_component() const
        {
            component_manager->register_component<T>();
        }

        template<typename T, typename... Args>
        T& add_component(EntityID entity, Args&&... args)
        {
            // Add the component to the entity
            T& component = component_manager->add_component<T>(entity, std::forward<Args>(args)...);

            // Update the entity's signature
            auto signature = entity_manager->get_signature(entity);
            signature.set(component_manager->get_component_type_id<T>(), true);
            entity_manager->set_signature(entity, signature);

            // Notify the system manager that the signature has changed
            system_manager->entity_signature_changed(entity, signature);
            
            return component;
        }

        template<typename T>
        void remove_component(EntityID entity) const
        {
            component_manager->remove_component<T>(entity);

            // Update the entity's signature
            auto signature = entity_manager->get_signature(entity);
            signature.set(component_manager->get_component_type_id<T>(), false);
            entity_manager->set_signature(entity, signature);

            // Notify the system manager that the signature has changed
            system_manager->entity_signature_changed(entity, signature);
        }

        template<typename T>
        T& get_component(EntityID entity)
        {
            return component_manager->get_component<T>(entity);
        }

        template<typename T>
        ComponentTypeID get_component_type_id() const
        {
            return component_manager->get_component_type_id<T>();
        }

        template<typename T>
        bool has_component(EntityID entity) const
        {
            return component_manager->has_component<T>(entity);
        }

        // --- System Methods ---
        template<typename T, typename... Args>
        void register_system(Args&&... args)
        {
            system_manager->register_system<T>(std::forward<Args>(args)...);
        }

        template<typename T>
        void set_system_signature(Signature signature) const
        {
            system_manager->set_signature<T>(signature);
        }

        // --- Update Method ---
        void update(double delta_time)
        {
            system_manager->update_all_systems(*this, delta_time);
        }

    private:
        std::unique_ptr<EntityManager> entity_manager;
        std::unique_ptr<ComponentManager> component_manager;
        std::unique_ptr<SystemManager> system_manager;
    };
}
