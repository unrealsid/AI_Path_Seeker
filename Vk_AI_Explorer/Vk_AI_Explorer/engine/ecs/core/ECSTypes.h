#pragma once

#include <cstdint>
#include <bitset>

namespace ecs::core
{
    using EntityID = uint32_t;
    using ComponentTypeID = uint32_t;
    
    constexpr EntityID INVALID_ENTITY = 0;
    constexpr size_t MAX_COMPONENTS = 64;
    constexpr size_t MAX_ENTITIES = 10000;
    
    using Signature = std::bitset<MAX_COMPONENTS>;
}
