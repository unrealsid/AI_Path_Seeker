#pragma once

#pragma once
#include <vulkan/vulkan.h>
#include <string>

namespace ecs::components
{
    struct Material
    {
        uint32_t material_index = 0;
        std::string material_name;
        VkDescriptorSet texture_descriptor_set = VK_NULL_HANDLE;
        bool needs_update = true;
    };
}