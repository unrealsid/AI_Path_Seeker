#pragma once

#include "../interfaces/ISystem.h"
#include "../Components/Material.h"
#include "../structs/EngineContext.h"

namespace systems
{
    class MaterialSystem : public ecs::core::ISystem
    {
    public:
        MaterialSystem(EngineContext& engine_context) : engine_context(engine_context) {}

        void update(ecs::core::World& world, double delta_time) override;

    private:
        EngineContext& engine_context;

        void update_material(ecs::components::Material& material);
    };
}
