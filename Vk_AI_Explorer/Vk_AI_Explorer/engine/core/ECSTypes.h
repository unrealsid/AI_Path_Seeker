#pragma once

#include <cstdint>
#include <bitset>

#define ECS_MAX_COMPONENTS 64
#define ECS_MAX_ENTITIES 10000

namespace ecs::core
{
    using EntityID = uint32_t;
    using ComponentTypeID = uint32_t;
    
    constexpr EntityID INVALID_ENTITY = 0;
    constexpr size_t MAX_COMPONENTS = ECS_MAX_COMPONENTS;
    constexpr size_t MAX_ENTITIES = ECS_MAX_ENTITIES;
    
    using ComponentMask = std::bitset<ECS_MAX_COMPONENTS>;
}
