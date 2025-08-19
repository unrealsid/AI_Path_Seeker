#pragma once

#include "../interfaces/ISystem.h"
#include "../Components/TransformComponent.h"
#include "../structs/EngineContext.h"
#include <glm/glm.hpp>

namespace ecs::systems
{
    class TransformSystem : public core::ISystem
    {
    public:
        
        TransformSystem() {}

        // ISystem interface implementation
        void init(core::World& world) override;
        void update(core::World& world, double delta_time) override;
        void cleanup(core::World& world) override;

        // Transform operations - these work with World reference
        static void set_position(core::World& world, core::EntityID entity, const glm::vec3& position);
        static void set_rotation(core::World& world, core::EntityID entity, const glm::vec3& euler_angles);
        static void set_rotation(core::World& world, core::EntityID entity, const glm::quat& rotation);
        static void set_scale(core::World& world, core::EntityID entity, const glm::vec3& scale);
    
        static glm::vec3 get_position(core::World& world, core::EntityID entity);
        static glm::vec3 get_world_position(core::World& world, core::EntityID entity);
        static glm::quat get_rotation(core::World& world, core::EntityID entity);
        static glm::vec3 get_rotation_euler(core::World& world, core::EntityID entity);
        static glm::vec3 get_scale(core::World& world, core::EntityID entity);
        static void set_parent(core::World& world, core::EntityID entity, core::EntityID parent);
        static void add_child(core::World& world, core::EntityID parent, core::EntityID child);
        static void remove_child(core::World& world, core::EntityID parent, core::EntityID child);
        static void remove_from_parent(core::World& world, core::EntityID entity);

        // Matrix operations
        static glm::mat4 get_local_matrix(core::World& world, core::EntityID entity);
        static glm::mat4 get_world_matrix(core::World& world, core::EntityID entity);
    
        // Debug output
        static void print_transform(core::World& world, core::EntityID entity);

    private:

        // Internal helper methods
        static void update_local_matrix(core::World& world, core::EntityID entity);
        static void update_world_matrix(core::World& world, core::EntityID entity);
        static void mark_dirty(core::World& world, core::EntityID entity);
        static void mark_children_dirty(core::World& world, core::EntityID entity);
    };
}
