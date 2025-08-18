#pragma once

#include "../interfaces/ISystem.h"
#include <vector>
#include <memory>

#include "World.h"

namespace ecs::core
{
    class SystemManager
    {
    public:
        template<typename T, typename... Args>
        T* add_system(Args&&... args)
        {
            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            T* system_ptr = system.get();
            systems.push_back(std::move(system));
            return system_ptr;
        }

        void init_all_systems(World& world)
        {
            for (auto& system : systems)
            {
                system->init(world);
            }
        }

        void update_all_systems(World& world, double delta_time)
        {
            for (auto& system : systems)
            {
                system->update(world, delta_time);
            }
        }

        void cleanup_all_systems(World& world)
        {
            for (auto& system : systems)
            {
                system->cleanup(world);
            }
        }

    private:
        std::vector<std::unique_ptr<ISystem>> systems;
    };
}
