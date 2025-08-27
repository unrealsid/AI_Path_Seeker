#pragma once

#include "../core/ECSTypes.h"

namespace ecs::interface
{
    class IComponentPool
    {
    public:
        virtual ~IComponentPool() = default;
        virtual void remove_component(core::EntityID entity) = 0;
        virtual bool has_component(core::EntityID entity) const = 0;
    };
}