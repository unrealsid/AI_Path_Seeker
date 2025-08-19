#pragma once

#include "../core/ECSTypes.h"

namespace ecs::core
{
    class IComponentPool
    {
    public:
        virtual void remove_component(EntityID entity) = 0;
        virtual bool has_component(EntityID entity) const = 0;
    };
}