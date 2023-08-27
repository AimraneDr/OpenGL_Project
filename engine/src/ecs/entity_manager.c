#include "ecs/entity_manager.h"

#include "core/CDS/dynamic_array.h"
#include "core/logger.h"

bool entity_manager_init(EntityManager* manager){
    manager->freeIds = dynamic_array_reserve(EntityID, MAX_ENTITIES);

    for(EntityID e=0; e < MAX_ENTITIES; e++){
        manager->entitiesComponentMasks[e] = EMPTY_COMPONENT_MASK;
        dynamic_array_push(manager->freeIds, e);
    }
    manager->activeEntitiesCount = 0;
    return true;
}

EntityID entity_manager_create_entity(EntityManager* manager){
    EntityID e = INVALID_ENTITY_ID;
    if(dynamic_array_length(manager->freeIds) > 0) dynamic_array_pop(manager->freeIds, &e);
    if(e != INVALID_ENTITY_ID){
        manager->entitiesComponentMasks[e] = 0;
        manager->activeEntitiesCount++;
    }
    return e;
};

bool entity_manager_destroy_entity(EntityManager* manager, EntityID id){
    dynamic_array_push(manager->freeIds, id);
    manager->entitiesComponentMasks[id] = 0;
    manager->activeEntitiesCount--;
    return true;
}

Mask64 entity_manager_get_components_mask(EntityManager* manager, EntityID id){
    return manager->entitiesComponentMasks[id];
}

bool entity_manager_set_components_mask(EntityManager* manager, EntityID id, Mask64 mask){
    manager->entitiesComponentMasks[id] = mask;
    return true;
}

bool entity_manager_reset_component_mask(EntityManager* manager, EntityID id){
    manager->entitiesComponentMasks[id] = 0;
    return true;
}
bool entity_manager_entity_has_component(EntityManager* manager, EntityID id, Mask64 component_type_mask){
    return (manager->entitiesComponentMasks[id] & component_type_mask) ? true : false;
}

bool entity_manager_clean_up(EntityManager* manager){
    for(u32 i=0; i < MAX_ENTITIES; i++){
        manager->entitiesComponentMasks[i] = 0;
    }
    manager->activeEntitiesCount = 0;
    dynamic_array_destroy(manager->freeIds)    
    return true;
}