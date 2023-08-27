
#include "ecs/component.h"

#include "core/logger.h"
#include "stdlib.h"
#include "string.h"
#include "core/CDS/unoredered_map.h"

ComponentArray* component_array_create(ComponentType type, u32 Component_size){
    ComponentArray* arr = (ComponentArray*)malloc(sizeof(ComponentArray));
    arr->type = type;
    for(u64 i = 0; i< MAX_ENTITIES; i++){
        arr->components[i] = (void*)0;
    }
    return arr;
}

bool component_array_destroy(ComponentArray* ca){
    if(ca == null) return false;
    ca->type = 0;
    for(u64 i = 0; i< MAX_ENTITIES; i++){
        if(ca->components[i] != (void*)0){
            free(ca->components[i]);
            ca->components[i] = (void*)0;
        }
    }
    free(ca);
    return true;
}

bool component_array_add_data(ComponentArray* ca,EntityID id, const void* component){
    if(ca->components[id] != null){
        ERR("entity (%llu) already has an instance of this component", id);
        return false;
    }
    ca->components[id] = malloc(ca->componentSize);
    memcpy(ca->components[id],component,ca->componentSize);
    return true;
}

void* component_array_get_data(ComponentArray* ca,EntityID id){
    if(ca->components[id] == (void*)0){
        ERR("Entity has no instance of this component (ComponentType : %llu)", ca->type);
        return null;
    }
    return ca->components[id];
}

bool component_array_remove_data(ComponentArray* ca,EntityID id){
    if(ca->components[id] == 0){
        ERR("Entity Already has no instance of this component");
        return false;
    }
    free(ca->components[id]);
    ca->components[id] = (void*)0;
    return true;
}

bool component_array_entity_destroyed(ComponentArray* ca,EntityID id){
    //any detachment needed goes here
    free(ca->components[id]);
    ca->components[id] = null;
    return true;
}
