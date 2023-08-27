#include "ecs/ecs_manager.h"

#include "ecs/entity_manager.h"
#include "ecs/componenet_manager.h"
#include "ecs/system_manager.h"

#include "core/CDS/unoredered_map.h"
#include "core/logger.h"

ECSManager* ecs_global_manager_get_instance(){
    static ECSManager globalManager;
    return &globalManager;
}

bool ecs_manager_init(ECSManager* out_manager){
    if(out_manager == 0 || out_manager == null) out_manager = ecs_global_manager_get_instance();
    if(!entity_manager_init(&out_manager->entityManager)){
        ERR("Initializing the ECS system failde. problem initializing the entity manager.")
        return false;
    }
    if(!components_manager_init(&out_manager->componentManager)){
        ERR("Initializing the ECS system failde. problem initializing the component manager.")
        return false;
    }
    if(!system_manager_init(&out_manager->systemManager)){
        ERR("Initializing the ECS system failde. problem initializing the system manager.")
        return false;
    }
    INFO("ECS manager initialized successfully")
    return true;
}

bool ecs_manager_shutdown(ECSManager* manager){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    
    bool result = true;
    if(!entity_manager_clean_up(&manager->entityManager)){
        ERR("Could not clean up the entity manager.")
        result = false;
    }

    if(!components_manager_clean_up(&manager->componentManager)){
        ERR("Could not clean up the component manager.")
        result = false;
    }

    if(!system_manager_clean_up(&manager->systemManager)){
        ERR("Could not clean up the system manager.")
        result = false;
    }

    return result;
}

/////
// ENTITY
/////

EntityID ecs_manager_create_entity(ECSManager* manager){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();

    EntityID e = entity_manager_create_entity(&manager->entityManager);
    if(e == INVALID_ENTITY_ID) ERR("failed to allocate new EntityID, no more available entities.")
    return e;
}

bool ecs_manager_destroy_entity(ECSManager* manager, EntityID entity){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    
    bool result = true;
    if(!entity_manager_destroy_entity(&manager->entityManager, entity)){
        ERR("Failed destroying entity (%llu).", entity)
        result = false;
    }
    if(!component_manager_entity_destroyed(&manager->componentManager, entity)){
        ERR("Failed releasing entity (%llu) from Component list.", entity)
        result = false;
    }
    if(!system_manager_entity_destroyed(&manager->systemManager, entity)){
        ERR("Failed releasing entity (%llu) from system targets list.", entity)
        result = false;
    }
    return true;
}

/////
// COMPONENT
/////


bool ecs_manager_register_component(ECSManager* manager, const char* componentName, u32 component_size){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return component_manager_register_component(&manager->componentManager, componentName, component_size);
}


bool ecs_manager_add_component(ECSManager* manager, EntityID entity, const char* componentName,const void* component){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    ComponentType cType = ecs_manager_get_component_type(componentName);
    if(!component_manager_add_component(&manager->componentManager, cType, entity, component))
         return false;

    Mask64 componentMask = component_manager_get_component_type_mask(cType);
    Mask64 newSignature = entity_manager_get_components_mask(&manager->entityManager, entity) | componentMask;
    entity_manager_set_components_mask(&manager->entityManager, entity, newSignature);

    system_manager_entity_signature_changed(&manager->systemManager, entity, newSignature);
    return true;
}


bool ecs_manager_remove_component(ECSManager* manager, EntityID entity, const char* componentName){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();

    ComponentType cType = ecs_manager_get_component_type(componentName);
    if(!entity_manager_entity_has_component(&manager->entityManager, entity, cType)) 
        return false;

    if(!component_manager_remove_component(&manager->componentManager, cType, entity))
         return false;

    Mask64 componentMask = component_manager_get_component_type_mask(cType);
    Mask64 newSignature = entity_manager_get_components_mask(&manager->entityManager, entity) ^ componentMask;
    entity_manager_set_components_mask(&manager->entityManager, entity, newSignature);

    system_manager_entity_signature_changed(&manager->systemManager, entity, newSignature);
    return true;
}


void* ecs_manager_get_component(ECSManager* manager, EntityID entity, const char* componentName){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return component_manager_get_component(&manager->componentManager, ecs_manager_get_component_type(componentName), entity);
}

ComponentType ecs_manager_get_component_type(const char* componentName){
    return component_manager_get_component_type(componentName);
}

Mask64 ecs_manager_get_component_type_signature(const char* componentName){
    return component_manager_get_component_type_mask(component_manager_get_component_type(componentName));
}

bool ecs_manager_entity_has_component(ECSManager* manager, EntityID entity, const char* componentName){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return entity_manager_entity_has_component(&manager->entityManager, entity, ecs_manager_get_component_type_signature(componentName));
}


/////
// SYSTEM
/////


bool ecs_manager_register_system(ECSManager* manager, SystemRegisterInfo register_info){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return system_manager_register_system(&manager->systemManager, register_info);    
}


bool ecs_manager_set_system_signature(ECSManager* manager, const char* systemName, Mask64 signature){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return system_manager_set_system_signature(&manager->systemManager, systemName, signature);
}


bool ecs_manager_systems_start(ECSManager* manager){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return system_manager_start_systems(&manager->systemManager);
}

bool ecs_manager_systems_update(ECSManager* manager, FrameInfo* frame){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return system_manager_update_systems(&manager->systemManager, frame);
}

bool ecs_manager_systems_shutdown(ECSManager* manager){
    if(manager == 0 || manager == null) manager = ecs_global_manager_get_instance();
    return system_manager_shutdown_systems(&manager->systemManager);
}