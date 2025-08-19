#include "TransformSystem.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

void ecs::systems::TransformSystem::init(core::World& world)
{
    // System initialization - nothing special needed for transforms
    std::cout << "TransformSystem initialized\n";
}

void ecs::systems::TransformSystem::update(core::World& world, double delta_time)
{
    // Get all entities with transform components
    auto entities = world.query_entities<components::TransformComponent>();
    
    if (entities.empty())
        return;
    
    auto* pool = world.get_component_pool<components::TransformComponent>();
    if (!pool)
        return;
    
    // First pass: update local matrices for dirty transforms
    for (core::EntityID entity : entities)
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        if (transform.dirty)
        {
            update_local_matrix(world, entity);
        }
    }
    
    // Second pass: update world matrices (process root entities first)
    for (core::EntityID entity : entities)
    {
        const auto& transform = world.get_component<components::TransformComponent>(entity);
        if (transform.parent == core::INVALID_ENTITY) // Root entity
        {
            update_world_matrix(world, entity);
        }
    }
}

void ecs::systems::TransformSystem::cleanup(core::World& world)
{
    std::cout << "TransformSystem cleanup\n";
}

void ecs::systems::TransformSystem::set_position(core::World& world, core::EntityID entity, const glm::vec3& position)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        transform.position = position;
        mark_dirty(world, entity);
    }
}

void ecs::systems::TransformSystem::set_rotation(core::World& world, core::EntityID entity, const glm::vec3& euler_angles)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        glm::vec3 radians = glm::radians(euler_angles);
        transform.rotation = glm::quat(radians);
        mark_dirty(world, entity);
    }
}

void ecs::systems::TransformSystem::set_rotation(core::World& world, core::EntityID entity, const glm::quat& rotation)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        transform.rotation = rotation;
        mark_dirty(world, entity);
    }
}

void ecs::systems::TransformSystem::set_scale(core::World& world, core::EntityID entity, const glm::vec3& scale)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        transform.scale = scale;
        mark_dirty(world, entity);
    }
}

glm::vec3 ecs::systems::TransformSystem::get_position(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        return world.get_component<components::TransformComponent>(entity).position;
    }
    return glm::vec3(0.0f);
}

glm::vec3 ecs::systems::TransformSystem::get_world_position(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        return world.get_component<components::TransformComponent>(entity).world_position;
    }
    return glm::vec3(0.0f);
}

glm::quat ecs::systems::TransformSystem::get_rotation(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        return world.get_component<components::TransformComponent>(entity).rotation;
    }
    return glm::quat();
}

glm::vec3 ecs::systems::TransformSystem::get_rotation_euler(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        return glm::eulerAngles(world.get_component<components::TransformComponent>(entity).rotation);
    }
    return glm::vec3(0.0f);
}

glm::vec3 ecs::systems::TransformSystem::get_scale(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        return world.get_component<components::TransformComponent>(entity).scale;
    }
    return glm::vec3(1.0f);
}

void ecs::systems::TransformSystem::set_parent(core::World& world, core::EntityID entity, core::EntityID parent)
{
    if (!world.has_component<components::TransformComponent>(entity))
        return;
        
    auto& transform = world.get_component<components::TransformComponent>(entity);
    
    // Remove from old parent
    if (transform.parent != core::INVALID_ENTITY)
    {
        remove_child(world, transform.parent, entity);
    }
    
    // Set new parent
    transform.parent = parent;
    
    // Add to new parent's children
    if (parent != core::INVALID_ENTITY && world.has_component<components::TransformComponent>(parent))
    {
        add_child(world, parent, entity);
    }
    
    mark_dirty(world, entity);
}

void ecs::systems::TransformSystem::add_child(core::World& world, core::EntityID parent, core::EntityID child)
{
    if (world.has_component<components::TransformComponent>(parent))
    {
        auto& parent_transform = world.get_component<components::TransformComponent>(parent);
        auto it = std::find(parent_transform.children.begin(), parent_transform.children.end(), child);
        if (it == parent_transform.children.end())
        {
            parent_transform.children.push_back(child);
        }
    }
}

void ecs::systems::TransformSystem::remove_child(core::World& world, core::EntityID parent, core::EntityID child)
{
    if (world.has_component<components::TransformComponent>(parent))
    {
        auto& parent_transform = world.get_component<components::TransformComponent>(parent);
        auto it = std::find(parent_transform.children.begin(), parent_transform.children.end(), child);
        if (it != parent_transform.children.end())
        {
            parent_transform.children.erase(it);
        }
    }
}

void ecs::systems::TransformSystem::remove_from_parent(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        if (transform.parent != core::INVALID_ENTITY)
        {
            remove_child(world, transform.parent, entity);
            transform.parent = core::INVALID_ENTITY;
            mark_dirty(world, entity);
        }
    }
}

glm::mat4 ecs::systems::TransformSystem::get_local_matrix(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        return world.get_component<components::TransformComponent>(entity).local_matrix;
    }
    return glm::mat4(1.0f);
}

glm::mat4 ecs::systems::TransformSystem::get_world_matrix(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        return world.get_component<components::TransformComponent>(entity).world_matrix;
    }
    return glm::mat4(1.0f);
}

void ecs::systems::TransformSystem::update_local_matrix(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        glm::mat4 T = glm::translate(glm::mat4(1.0f), transform.position);
        glm::mat4 R = glm::toMat4(transform.rotation);
        glm::mat4 S = glm::scale(glm::mat4(1.0f), transform.scale);
        transform.local_matrix = T * R * S;
    }
}

void ecs::systems::TransformSystem::update_world_matrix(core::World& world, core::EntityID entity)
{
    if (!world.has_component<components::TransformComponent>(entity))
        return;
        
    auto& transform = world.get_component<components::TransformComponent>(entity);
    
    if (transform.parent != core::INVALID_ENTITY && world.has_component<components::TransformComponent>(transform.parent))
    {
        // Has parent - multiply by parent's world matrix
        const auto& parent_transform = world.get_component<components::TransformComponent>(transform.parent);
        transform.world_matrix = parent_transform.world_matrix * transform.local_matrix;
    }
    else
    {
        // No parent - world matrix is local matrix
        transform.world_matrix = transform.local_matrix;
    }
    
    // Update cached world position
    transform.world_position = glm::vec3(transform.world_matrix[3]);
    transform.dirty = false;
    
    // Recursively update children
    for (core::EntityID child : transform.children)
    {
        if (world.has_component<components::TransformComponent>(child))
        {
            update_world_matrix(world, child);
        }
    }
}

void ecs::systems::TransformSystem::mark_dirty(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        auto& transform = world.get_component<components::TransformComponent>(entity);
        transform.dirty = true;
        mark_children_dirty(world, entity);
    }
}

void ecs::systems::TransformSystem::mark_children_dirty(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        const auto& transform = world.get_component<components::TransformComponent>(entity);
        for (core::EntityID child : transform.children)
        {
            if (world.has_component<components::TransformComponent>(child))
            {
                auto& child_transform = world.get_component<components::TransformComponent>(child);
                child_transform.dirty = true;
                mark_children_dirty(world, child);
            }
        }
    }
}

void ecs::systems::TransformSystem::print_transform(core::World& world, core::EntityID entity)
{
    if (world.has_component<components::TransformComponent>(entity))
    {
        const auto& transform = world.get_component<components::TransformComponent>(entity);
        glm::vec3 euler = glm::degrees(glm::eulerAngles(transform.rotation));
        std::cout << "Entity " << entity << " Transform { "
                  << "Position: (" << transform.world_position.x << ", " << transform.world_position.y << ", " << transform.world_position.z << "), "
                  << "Rotation: (" << euler.x << "°, " << euler.y << "°, " << euler.z << "°), "
                  << "Scale: (" << transform.scale.x << ", " << transform.scale.y << ", " << transform.scale.z << ") }";
                  
        const std::string& tag = world.get_entity_tag(entity);
        if (!tag.empty())
        {
            std::cout << " [" << tag << "]";
        }
        std::cout << std::endl;
    }
}