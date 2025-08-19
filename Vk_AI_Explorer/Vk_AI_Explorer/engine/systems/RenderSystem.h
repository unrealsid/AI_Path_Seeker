#pragma once

#include "../interfaces/ISystem.h"
#include "../Components/Renderable.h"
#include "../Components/Material.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawBatch.h"
#include "../structs/EngineContext.h"
#include <unordered_map>

namespace ecs::systems
{
    class RenderSystem : public ecs::core::ISystem
    {
    public:
        RenderSystem(EngineContext& engine_context) : engine_context(engine_context) {}

        void update(ecs::core::World& world, double delta_time) override;

    private:
        EngineContext& engine_context;
        std::unordered_map<std::string, ecs::components::DrawBatch> draw_batches;

        void organize_draw_batches(ecs::core::World& world);
        void render_entities(ecs::core::World& world);
        void render_entity(ecs::core::World& world, ecs::core::EntityID entity, const ecs::components::Renderable& renderable);
    };
}
