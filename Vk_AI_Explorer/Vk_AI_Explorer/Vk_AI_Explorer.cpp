//

#include "Vk_AI_Explorer.h"

#include <iostream>

#include "engine/components/TransformComponent.h"
#include "engine/Core/SystemManager.h"
#include "engine/Core/World.h"
#include "engine/systems/TransformSystem.h"

namespace ecs::systems
{
	class TransformSystem;
}

int main()
{
	using namespace ecs::core;
    
	World world;
	ecs::core::SystemManager system_manager;
    
	// Add the transform system
	auto* transform_system = system_manager.add_system<ecs::systems::TransformSystem>();
	system_manager.init_all_systems(world);
    
	// Create entities
	EntityID parent = world.create_entity("Parent");
	EntityID child = world.create_entity("Child");
    
	// Add transform components
	world.add_component<ecs::components::TransformComponent>(parent, glm::vec3(10, 0, 0));
	world.add_component<ecs::components::TransformComponent>(child, glm::vec3(5, 0, 0));
    
	// Set up hierarchy
	ecs::systems::TransformSystem::set_parent(world, child, parent);
    
	// Update systems
	system_manager.update_all_systems(world, 0.016); // 60 FPS
    
	// Print results
	ecs::systems::TransformSystem::print_transform(world, parent);
	ecs::systems::TransformSystem::print_transform(world, child);
    
	system_manager.cleanup_all_systems(world);

	return 0;
}
