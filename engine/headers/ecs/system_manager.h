#ifndef ECS_SYSTEM_MANAGER_H
#define ECS_SYSTEM_MANAGER_H

#include "defines.h"
#include "ecs_defines.h"
#include "engine_types.h"


bool system_manager_init(SystemManager* out_manager);
bool system_manager_clean_up(SystemManager* manager);
bool system_manager_register_system(SystemManager* manager, SystemRegisterInfo register_info);
bool system_manager_set_system_signature(SystemManager* manager,const char* sys_name, Mask64 signature);
bool system_manager_entity_destroyed(SystemManager* manager, EntityID id);
bool system_manager_entity_signature_changed(SystemManager* manager, EntityID id, Mask64 entitySignature);


bool system_manager_start_systems(SystemManager* manager);
bool system_manager_update_systems(SystemManager* manager, FrameInfo* frame);
bool system_manager_shutdown_systems(SystemManager* manager);



#endif /* ECS_SYSTEM_MANAGER_H */