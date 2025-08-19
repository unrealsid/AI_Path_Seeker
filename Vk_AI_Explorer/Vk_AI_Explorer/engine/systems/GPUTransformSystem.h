#pragma once

#include "../interfaces/ISystem.h"
#include "../Components/GPUTransform.h"
#include "../Components/TransformComponent.h"
#include "../structs/EngineContext.h"

namespace systems
{
    class GPUTransformSystem : public ecs::core::ISystem
    {
    public:
        GPUTransformSystem(EngineContext& engine_context) : engine_context(engine_context) {}

        void update(ecs::core::World& world, double delta_time) override;

    private:
        EngineContext& engine_context;

        void update_transform_on_gpu(ecs::components::GPUTransform& gpu_transform, const glm::mat4& matrix);
    };
}
