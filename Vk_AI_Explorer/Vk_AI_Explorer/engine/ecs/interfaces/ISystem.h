#pragma once

#include <set>

namespace ecs::core
{
    class World;
}

namespace ecs::interfaces
{
    class ISystem
    {
    public:
        virtual ~ISystem() = default;
       
        virtual void init(core::World& world) {}
        virtual void update(core::World& world, double delta_time) {}
        virtual void cleanup(core::World& world) {}
        
        std::set<core::EntityID> entities;
    };
}
