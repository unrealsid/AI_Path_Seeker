#pragma once

#include "ECSTypes.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <cassert>

namespace ecs::core
{
    class EntityManager
    {
    public:
        EntityManager()
        {
            // Initialize the queue with all possible entity IDs.
            for (EntityID entity = 1; entity < 10000; ++entity)
            {
                available_entities.push(entity);
            }
        }

        // Creates a new entity and returns its ID.
        EntityID create_entity()
        {
            assert(!available_entities.empty() && "Maximum number of entities exceeded.");

            // Reuse an old ID if available
            EntityID id = available_entities.front();
            available_entities.pop();
            return id;
        }

        // Creates a new entity and returns its ID.
        // Overloaded to also accept a name.
        EntityID create_entity(const std::string& name )
        {
            assert(!available_entities.empty() && "Maximum number of entities exceeded.");
            
            EntityID id = available_entities.front();
            available_entities.pop();

            if (!name.empty())
            {
                // Ensure the name is unique
                assert(entity_names_to_id.contains(name) && "Entity name must be unique.");
                entity_names_to_id[name] = id;
                entity_id_to_names[id] = name;
            }

            return id;
        }


        // Destroys an entity, making its ID available for reuse.
        void destroy_entity(EntityID entity)
        {
            assert(entity != INVALID_ENTITY && "Cannot destroy an invalid entity.");

            // Reset its signature and put the ID back in the queues
            entity_signatures.erase(entity);
            available_entities.push(entity);
        }

        // Retrieves an entity ID by its name.
        EntityID get_entity_by_name(const std::string& name) const
        {
            auto it = entity_names_to_id.find(name);
            if (it != entity_names_to_id.end())
            {
                return it->second;
            }
            return INVALID_ENTITY;
        }

        // Sets the signature for a given entity.
        void set_signature(EntityID entity, Signature signature)
        {
            entity_signatures[entity] = signature;
        }

        // Retrieves the signature for a given entity.
        Signature get_signature(EntityID entity)
        {
            // Return the entity's signature or an empty one if it doesn't exist
            return entity_signatures.count(entity) ? entity_signatures[entity] : Signature{};
        }

    private:
        // A queue of available (destroyed) entity IDs to be recycled.
        std::queue<EntityID> available_entities;
        // A map from entity ID to its component signature.
        std::unordered_map<EntityID, Signature> entity_signatures;
        // A map from entity name to its entity ID.
        std::unordered_map<std::string, EntityID> entity_names_to_id;
        //Reverse lookup for id to names
        std::unordered_map<EntityID, std::string> entity_id_to_names;
    };
}
