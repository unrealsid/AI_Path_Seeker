#pragma once

#include "ECSTypes.h"

namespace ecs::core
{
    class ComponentRegistry
    {
    public:
        template<typename T>
        static ComponentTypeID get_component_type_id()
        {
            static ComponentTypeID id = next_component_id++;
            return id;
        }
        
    private:
        inline static ComponentTypeID next_component_id = 0;
    };
}
