#pragma once

#include "../core/World.h"

namespace ecs::core
{
    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        virtual void update(World& world, double delta_time) = 0;
        virtual void init(World& world) {}
        virtual void cleanup(World& world) {}
    };
}
