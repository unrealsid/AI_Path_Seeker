#pragma once

#include "Core/World.h"
#include "Core/SystemManager.h"
#include "Systems/TransformSystem.h"
#include "Systems/MaterialSystem.h"
#include "Systems/GPUTransformSystem.h"
#include "Systems/RenderSystem.h"
#include "structs/Vk_RenderData.h"

namespace ecs::engine
{
    class ECSEngine
    {
    public:
        static ECSEngine& get_instance()
        {
            static ECSEngine instance;
            return instance;
        }

        void init(EngineContext& engine_context);
        void run();
        void cleanup();

        core::World& get_world() { return world; }
        EngineContext* get_engine_context() { return engine_context; }

        // Helper functions for creating common entity types
        core::EntityID create_renderable_entity(const std::string& tag, const RenderData& render_data);
        core::EntityID create_camera_entity(const std::string& tag);

    private:
        core::World world;
        core::SystemManager system_manager;
        EngineContext* engine_context = nullptr;

        ECSEngine() = default;
        ~ECSEngine() = default;
        ECSEngine(const ECSEngine&) = delete;
        ECSEngine& operator=(const ECSEngine&) = delete;

        void load_models();
    };
}
