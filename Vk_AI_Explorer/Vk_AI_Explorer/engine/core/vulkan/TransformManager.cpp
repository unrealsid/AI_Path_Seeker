#include "TransformManager.h"

#include <glm/fwd.hpp>

#include "engine/core/utils/MemoryUtils.h"
#include "engine/core/structs//EngineContext.h"
#include "../vulkan/DeviceManager.h"

core::TransformManager::TransformManager(EngineContext& engine_context):engine_context(engine_context)
{
    allocate_transform_buffer();
}

void core::TransformManager::allocate_transform_buffer()
{
    utils::MemoryUtils::allocate_buffer_with_random_access(engine_context.dispatch_table, engine_context.device_manager->get_allocator(), 5000 * sizeof(glm::mat4), transforms_buffer);
}
