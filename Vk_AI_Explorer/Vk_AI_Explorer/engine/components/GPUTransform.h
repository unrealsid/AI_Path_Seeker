#pragma once

#include "../structs/GPU_Buffer.h"
#include <vulkan/vulkan.h>

namespace ecs::components
{
    struct GPUTransform
    {
        VkDeviceAddress transform_buffer_address = 0;
        VkDeviceAddress object_id_buffer_address = 0;
        GPU_Buffer object_id_buffer;
        bool needs_update = true;
    };
}
