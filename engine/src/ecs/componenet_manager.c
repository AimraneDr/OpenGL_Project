#include "ecs/componenet_manager.h"

#include "ecs/component.h"
#include "core/logger.h"
#include "core/CDS/details/hash.h"
#include "core/CDS/strings.h"

#include <stdlib.h>
#include <string.h>

u32 component_mask_to_array_index(Mask64 mask){
    if(mask == 0) return -1;
    u32 index = 0;
    while(mask != 1){
        mask >>= 1;
        index++;
    }
    return index;
}

bool components_manager_init(ComponentManager* out_manager){
    out_manager->componentsCount = 0;
    
    for(u32 i=0; i < MAX_COMPONENT_TYPES; i++)  out_manager->ComponentsArrays[i] = null;
    
    return true;
}

bool components_manager_clean_up(ComponentManager* manager){
    for(u32 i=0; i < MAX_COMPONENT_TYPES; i++){
        component_array_destroy(manager->ComponentsArrays[i]);
    }
    manager->componentsCount = 0;
    return true;
}

bool component_manager_register_component(ComponentManager* manager,const char* component_name, u32 component_size){
    ComponentType type = component_manager_get_component_type(component_name);
    
    if(manager->ComponentsArrays[type] != 0){
        if(stringCompare(manager->ComponentsArrays[type]->name, component_name)){
            ERR("Component of type '%s' already registered.",component_name)
            return false;    
        }
        ERR("components ['%s' , '%s'] have a conflict type. Could not register '%2$s'.", manager->ComponentsArrays[type]->name, component_name)
        return false;
    }

    // TODO : test (passing the address of ComponentArray insteade of the value as the value of the key in the unordered map)
    
    ComponentArray* cArr = component_array_create(type, component_size);
    cArr->name = malloc(stringLength(component_name) + 1);
    stringCopy(cArr->name, component_name);

    manager->ComponentsArrays[type] = cArr;
    manager->componentsCount++;
    return true;
}
Mask64 component_manager_get_component_type_mask(ComponentType component_type){
    Mask64 m = 1 << component_type;
    return m;
}

ComponentType component_manager_get_component_type(const char* component_name){
    ComponentType type = (ComponentType)hash_string(component_name, MAX_COMPONENT_TYPES);
    return type;
}

/// @brief 
/// @param manager 
/// @param component_type 
/// @param id 
/// @return a void* to component, cast back to desired type
void* component_manager_get_component(ComponentManager* manager, ComponentType component_type, EntityID id){
    ComponentArray* arr = manager->ComponentsArrays[component_type];
    if(arr == 0 || arr->type == 0){
        ERR("Attempt of retrieving an unregistered Component from an entity")
        return null;
    }
    if(arr->aliveComponentsCount == 0){
        ERR("Attempt of retrieving a components from an empty componentArray")
        return null;
    }
    return component_array_get_data(arr,id);
}

bool component_manager_add_component(ComponentManager* manager, ComponentType component_type, EntityID id, const void* component_data){
    ComponentArray* arr = manager->ComponentsArrays[component_type];

    if(arr == 0 || arr->type == 0){
        ERR("Attempt of adding an unregistered Component to an entity")
        return false;
    }
    return component_array_add_data(arr, id, component_data);
}

bool component_manager_remove_component(ComponentManager* manager, ComponentType component_type, EntityID id){
    ComponentArray* arr = manager->ComponentsArrays[component_type];
    if(arr == 0 || arr->type == 0){
        ERR("Attempt of removing an unregistered Component from an entity")
        return false;
    }
    return component_array_remove_data(arr, id);
}

bool component_manager_entity_destroyed(ComponentManager* manager, EntityID id){
    for(u32 i=0; i<MAX_COMPONENT_TYPES; i++){
        component_array_remove_data(manager->ComponentsArrays[i], id);
    }
    return true;
}
