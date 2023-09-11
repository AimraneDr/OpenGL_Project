#ifndef ECS_COMPONENT_H
#define ECS_COMPONENT_H

#include "ecs_defines.h"
#include "defines.h"

ComponentArray* component_array_create(ComponentType type, u32 Component_size);
bool component_array_destroy(ComponentArray* ca);
bool component_array_add_data(ComponentArray* ca,EntityID id, const void* component);
void* component_array_get_data(ComponentArray* ca,EntityID id);
bool component_array_remove_data(ComponentArray* ca,EntityID id);
bool component_array_entity_destroyed(ComponentArray* ca,EntityID id);

#endif /* ECS_COMPONENT_H */