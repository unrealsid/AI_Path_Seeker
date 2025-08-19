#pragma once

#include "ECSTypes.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace ecs::core
{
    class EntityManager
    {
    public:
        EntityID create_entity()
        {
            EntityID entity;
            if (!available_entities.empty())
            {
                entity = available_entities.back();
                available_entities.pop_back();
            }
            else
            {
                entity = next_entity_id++;
            }
            
            entity_masks[entity] = ComponentMask{};
            entity_tags[entity] = "";
            return entity;
        }

        EntityID create_entity(const std::string& tag)
        {
            EntityID entity = create_entity();
            entity_tags[entity] = tag;
            tag_to_entity[tag] = entity;
            return entity;
        }

        void destroy_entity(EntityID entity)
        {
            if (entity_tags.find(entity) != entity_tags.end())
            {
                tag_to_entity.erase(entity_tags[entity]);
                entity_tags.erase(entity);
            }
            
            entity_masks.erase(entity);
            available_entities.push_back(entity);
        }

        ComponentMask& get_component_mask(EntityID entity)
        {
            return entity_masks[entity];
        }

        const ComponentMask& get_component_mask(EntityID entity) const
        {
            return entity_masks.at(entity);
        }

        EntityID get_entity_by_tag(const std::string& tag) const
        {
            auto it = tag_to_entity.find(tag);
            return (it != tag_to_entity.end()) ? it->second : INVALID_ENTITY;
        }

        const std::string& get_entity_tag(EntityID entity) const
        {
            static const std::string empty_tag;
            auto it = entity_tags.find(entity);
            return (it != entity_tags.end()) ? it->second : empty_tag;
        }

        std::vector<EntityID> query_entities(const ComponentMask& mask) const
        {
            std::vector<EntityID> result;
            for (const auto& pair : entity_masks)
            {
                if ((pair.second & mask) == mask)
                {
                    result.push_back(pair.first);
                }
            }
            return result;
        }

    private:
        EntityID next_entity_id = 1;
        std::vector<EntityID> available_entities;
        std::unordered_map<EntityID, ComponentMask> entity_masks;
        std::unordered_map<EntityID, std::string> entity_tags;
        std::unordered_map<std::string, EntityID> tag_to_entity;
    };
}
