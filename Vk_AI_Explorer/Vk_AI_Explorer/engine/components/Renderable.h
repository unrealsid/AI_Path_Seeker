#pragma once

#include <unordered_map>

#include "../structs/GPU_Buffer.h"
#include "../structs/Vertex.h"
#include <glm/glm.hpp>
#include <vector>

namespace ecs::components
{
    struct Renderable
    {
        GPU_Buffer vertex_buffer;
        GPU_Buffer index_buffer;
        glm::vec3 local_position{0.0f};
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
         std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> material_index_ranges;
        uint32_t material_index = 0;
        bool visible = true;
    };
}
