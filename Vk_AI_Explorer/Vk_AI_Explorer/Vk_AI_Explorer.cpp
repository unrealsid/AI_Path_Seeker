//

#include "Vk_AI_Explorer.h"

#include "engine/ecs/core/ECSTypes.h"
#include "engine/ecs/core/World.hpp"
#include "engine/ecs/components/TransformComponent.h" 
#include "engine/ecs/components/VelocityComponent.h"  
#include "engine/ecs/systems/MovementSystem.h"

#include <iostream>
#include <vector>
#include <random>      
#include <chrono>      
#include <thread>

int main()
{
    // Create an instance of the ECS world
    ecs::core::World world;

    // --- Component Registration ---
    // All components must be registered before use
    world.register_component<ecs::components::TransformComponent>();
    world.register_component<ecs::components::VelocityComponent>();

    // --- System Registration ---
    // Create and register the MovementSystem
    world.register_system<ecs::systems::MovementSystem>();

    // Define the signature for the MovementSystem
    // It requires both a Transform and a Velocity component
    ecs::core::Signature movement_signature;
    movement_signature.set(world.get_component_type_id<ecs::components::TransformComponent>());
    movement_signature.set(world.get_component_type_id<ecs::components::VelocityComponent>());
    world.set_system_signature<ecs::systems::MovementSystem>(movement_signature);

    // --- Entity Creation ---
    // Create 500 entities, each with a random velocity
    const int num_entities = 500;
    std::vector<ecs::core::EntityID> entities;

    // Random number generation for velocity vectors
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    std::cout << "Creating " << num_entities << " entities with random velocities..." << std::endl;

    for (int i = 0; i < num_entities; ++i)
    {
        // Create a new entity
        ecs::core::EntityID new_entity = world.create_entity();
        entities.push_back(new_entity);

        // Add a Transform component with an initial position
        world.add_component<ecs::components::TransformComponent>(new_entity,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );

        // Add a Velocity component with a random direction
        world.add_component<ecs::components::VelocityComponent>(new_entity,
            glm::vec3(dis(gen), dis(gen), dis(gen))
        );
    }

    // --- Simulation Loop ---
    // Set up the simulation timer
    const double simulation_duration_s = 5.0; // Run for 5 seconds
    
    // Get the starting time point for the simulation
    auto simulation_start_time = std::chrono::high_resolution_clock::now();
    
    // Get the time point for the start of the first frame
    auto last_frame_time = std::chrono::high_resolution_clock::now();

    std::cout << "\nStarting simulation for " << simulation_duration_s << " seconds..." << std::endl;

    while (true)
    {
        // Calculate the elapsed time since the start of the simulation
        auto current_simulation_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = current_simulation_time - simulation_start_time;

        // Break the loop if the simulation duration is reached
        if (elapsed_time.count() >= simulation_duration_s)
        {
            break;
        }

        // Dynamically calculate delta_time for this frame
        std::chrono::duration<double> frame_time = current_simulation_time - last_frame_time;
        double delta_time = frame_time.count();
        last_frame_time = current_simulation_time;

        // Update the world by one tick with the dynamically calculated delta_time
        world.update(delta_time);

        // Optional: Print the position of the first entity to show progress
        auto& first_entity_transform = world.get_component<ecs::components::TransformComponent>(entities[0]);
        std::cout << "\rElapsed: " << elapsed_time.count() << "s, Entity " << entities[0]
                  << " Position: (" << first_entity_transform.position.x << ", "
                  << first_entity_transform.position.y << ", "
                  << first_entity_transform.position.z << ")" << std::flush;
    }

    std::cout << "\n\nSimulation finished." << std::endl;
    
    // Print final positions of the first 5 entities
    std::cout << "\nFinal Positions:" << std::endl;
    for (int i = 0; i < 500; ++i)
    {
        auto& transform = world.get_component<ecs::components::TransformComponent>(entities[i]);
        std::cout << "Entity " << entities[i] << " Position: ("
            << transform.position.x << ", "
            << transform.position.y << ", "
            << transform.position.z << ")" << std::endl;
    }

    // --- Entity Destruction ---
    // Clean up all entities
    std::cout << "\nDestroying " << num_entities << " entities..." << std::endl;
    for (const auto& entity : entities)
    {
        world.destroy_entity(entity);
    }

    return 0;
}
