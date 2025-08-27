#pragma once

#include "ECSTypes.h"
#include <memory>
#include <unordered_map>

#include <typeinfo>
#include <cassert>

#include "../interfaces/ISystem.h"

namespace ecs::core
{
    class ISystem;
    class World;

    class SystemManager
    {
    public:
        // Registers a new system and returns a pointer to it.
        template<typename T, typename... Args>
        void register_system(Args&&... args)
        {
            const char* type_name = typeid(T).name();
            assert(!systems.contains(type_name) && "Registering system type more than once.");

            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            systems.insert({type_name, std::move(system)});
        }

        // Sets the component signature for a system.
        // A system will only operate on entities that have this signature.
        template<typename T>
        void set_signature(Signature signature)
        {
            const char* type_name = typeid(T).name();
            assert(systems.contains(type_name)&& "System used before registration.");
            system_signatures.insert({type_name, signature});
        }

        // Called when an entity's signature changes (component added/removed).
        // This method updates all systems to either include or exclude the entity.
        void entity_signature_changed(EntityID entity, Signature entity_signature);

        // Called when an entity is destroyed to remove it from all systems.
        void entity_destroyed(EntityID entity);

        // Calls the update method on all registered systems.
        void update_all_systems(World& world, double delta_time);

    private:
        // Map from system type name to its signature
        std::unordered_map<const char*, Signature> system_signatures;
        // Map from system type name to the system instance
        std::unordered_map<const char*, std::unique_ptr<ecs::interfaces::ISystem>> systems;
    };
}
