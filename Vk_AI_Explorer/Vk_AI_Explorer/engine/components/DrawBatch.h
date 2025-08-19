#pragma once
#include "../Core/ECSTypes.h"
#include <vector>
#include <string>

namespace ecs::components
{
    struct DrawBatch
    {
        std::string batch_name;
        std::vector<ecs::core::EntityID> entities;
        uint32_t vertex_offset = 0;
        uint32_t index_offset = 0;
        uint32_t vertex_count = 0;
        uint32_t index_count = 0;
    };
}