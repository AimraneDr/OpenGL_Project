#ifndef ECS_ENTITY_MANAGER_H
#define ECS_ENTITY_MANAGER_H

#include "ecs_defines.h"
#include "defines.h"

bool entity_manager_init(EntityManager* manager);
EntityID entity_manager_create_entity(EntityManager* manager);
bool entity_manager_destroy_entity(EntityManager* manager,EntityID id);
Mask64 entity_manager_get_components_mask(EntityManager* manager, EntityID id);
bool entity_manager_set_components_mask(EntityManager* manager, EntityID id, Mask64 mask);
bool entity_manager_reset_component_mask(EntityManager* manager, EntityID id);
bool entity_manager_entity_has_component(EntityManager* manager, EntityID id, Mask64 component_type_mask);
bool entity_manager_clean_up(EntityManager* manager);

#endif /* ECS_ENTITY_MANAGER_H */