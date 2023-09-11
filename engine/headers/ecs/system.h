#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include "ecs_defines.h"

System* system_create_new_system(SystemRegisterInfo info);
bool system_add_entity_to_targets_list(System* s, EntityID e);
bool system_is_targetting_entity(System* s, EntityID e);
bool system_remove_entity_from_targets_list(System* s, EntityID e);
bool system_clear_targets_list(System* s);
bool system_clean_up(System* s);

#endif /* ECS_SYSTEM_H */