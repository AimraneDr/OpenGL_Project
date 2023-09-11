#ifndef ECS_COMPONENT_MANAGER_H
#define ECS_COMPONENT_MANAGER_H

#include "defines.h"
#include "ecs_defines.h"

bool components_manager_init(ComponentManager* out_manager);
bool components_manager_clean_up(ComponentManager* manager);
bool component_manager_register_component(ComponentManager* manager,const char* component_name, u32 component_size);
void* component_manager_get_component(ComponentManager* manager, ComponentType component_type, EntityID id);
bool component_manager_add_component(ComponentManager* manager, ComponentType component_type, EntityID id, const void* component_data);
bool component_manager_remove_component(ComponentManager* manager, ComponentType component_type, EntityID id);
bool component_manager_entity_destroyed(ComponentManager* manager, EntityID id);

ComponentType component_manager_get_component_type(const char* component_name);
Mask64 component_manager_get_component_type_mask(ComponentType component_type);

#endif /* ECS_COMPONENT_MANAGER_H */