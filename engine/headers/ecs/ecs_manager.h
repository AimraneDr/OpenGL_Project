#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H

#include "defines.h"
#include "ecs_defines.h"
#include "engine_types.h"

/// @brief get the global instance of the ECSManager
/// @return 
DLL_EXPORT ECSManager* ecs_global_manager_get_instance();


/// @brief initialize ecs manager
/// @param out_manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @return 
DLL_EXPORT bool ecs_manager_init(ECSManager* out_manager);

/// @brief shutting down the ecs manager
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @return 
DLL_EXPORT bool ecs_manager_shutdown(ECSManager* manager);

/////
// ENTITY
/////

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @return a valid EntityID, or INVALID_ENTITY_ID if no more entities are available
DLL_EXPORT EntityID ecs_manager_create_entity(ECSManager* manager);
DLL_EXPORT bool ecs_manager_destroy_entity(ECSManager* manager, EntityID entity);

/////
// COMPONENT
/////

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param componentName use TYPE_NAME() from 'defines.h' to data type name as string
/// @return 
DLL_EXPORT bool ecs_manager_register_component(ECSManager* manager, const char* componentNam, u32 component_sizee);

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param entity 
/// @param componentName use TYPE_NAME() from 'defines.h' to data type name as string
/// @param component component instance
/// @return 
DLL_EXPORT bool ecs_manager_add_component(ECSManager* manager, EntityID entity, const char* componentName, const void* component);

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param entity 
/// @param componentName use TYPE_NAME() from 'defines.h' to data type name as string
/// @return 
DLL_EXPORT bool ecs_manager_remove_component(ECSManager* manager, EntityID entity, const char* componentName);

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param entity 
/// @param componentName use TYPE_NAME() from 'defines.h' to data type name as string
/// @return 
DLL_EXPORT void* ecs_manager_get_component(ECSManager* manager, EntityID entity, const char* componentName);

/// @brief 
/// @param componentName use TYPE_NAME() from 'defines.h' to data type name as string
/// @return 
ComponentType ecs_manager_get_component_type(const char* componentName);

/// @brief 
/// @param componentName use TYPE_NAME() from 'defines.h' to data type name as string
/// @return 
Mask64 ecs_manager_get_component_type_signature(const char* componentName);

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param entity 
/// @param componentName use TYPE_NAME() from 'defines.h' to data type name as string
/// @return 
DLL_EXPORT bool ecs_manager_entity_has_component(ECSManager* manager, EntityID entity, const char* componentName);

#define ECSManagerGetComponentType(componentType) ecs_manager_get_component_type(TYPE_NAME(componentType))

/////
// SYSTEM
/////

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param register_info system's register info
/// @return 
DLL_EXPORT bool ecs_manager_register_system(ECSManager* manager, SystemRegisterInfo register_info);

/// @brief 
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param systemName use TYPE_NAME() from 'defines.h' to data type name as string
/// @param signature 
/// @return 
DLL_EXPORT bool ecs_manager_set_system_signature(ECSManager* manager, const char* systemName, Mask64 signature);

/// @brief run the SystemStartFunc for all registered systems
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @return 
bool ecs_manager_systems_start(ECSManager* manager);

/// @brief run the SystemUpdateFunc for all registered systems
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @param dt 
/// @return 
bool ecs_manager_systems_update(ECSManager* manager, FrameInfo* frame);

/// @brief run the SystemShutdownFunc for all registered systems
/// @param manager a ref to the ecs manager targeted, if 0 or null the function will target the global instance
/// @return 
bool ecs_manager_systems_shutdown(ECSManager* manager);

#endif /* ECS_MANAGER_H */