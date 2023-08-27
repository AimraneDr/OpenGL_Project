#include "ecs/system_manager.h"

#include "core/CDS/unoredered_map.h"
#include "core/logger.h"

bool system_manager_init(SystemManager* out_manager){
    unordered_map_create(&out_manager->mSystems,_S_MAP_SIZE, MAP_KEY_TYPE_STRING);
    out_manager->systemsCount = 0;
    return true;
}

bool system_manager_clean_up(SystemManager* manager){
    MapNode* node = manager->mSystems.start;
    while(node){
        free(node->value);
        node = node->next;
    }
    unordered_map_clean(&manager->mSystems);
    manager->systemsCount = 0;
    return true;
}

bool system_manager_register_system(SystemManager* manager, SystemRegisterInfo register_info){
    if(unordered_map_is_set(&manager->mSystems, (void*)register_info.systemName)){
        ERR("Attempt of registering a previously registered System");
        return false;
    }
    
    //resize if the map is filled
    if(manager->mSystems.length == manager->mSystems.maxSize) unordered_map_resize(&manager->mSystems, manager->mSystems.maxSize + (manager->mSystems.maxSize/4));
    //move to separate func so i can free it afterward
    System* sysPtr = (System*)malloc(sizeof(System));
    sysPtr->systemName = register_info.systemName;
    sysPtr->self = register_info.self;
    sysPtr->start = register_info.start;
    sysPtr->preUpdate = register_info.preUpdate;
    sysPtr->update = register_info.update;
    sysPtr->AfterUpdate = register_info.AfterUpdate;
    sysPtr->shutdown = register_info.shutdown;
    unordered_map_set(&manager->mSystems, (void*)register_info.systemName, (void*)sysPtr);

    manager->systemsCount++;
    return true;
}

bool system_manager_set_system_signature(SystemManager* manager,const char* sys_name, Mask64 signature){
    if(!unordered_map_is_set(&manager->mSystems, (void*)sys_name)){
        ERR("System is not registered, can't update the signature.")
        return false;
    }
    System* sys = unordered_map_get(&manager->mSystems, (void*)sys_name);
    sys->targetsSignature = signature;
    return true;
}

bool system_manager_entity_destroyed(SystemManager* manager, EntityID id){
    MapNode* node = manager->mSystems.start;
    while(node){
        System* s = node->value;
        s->targets[id / ENTITY_BIT_SIZE] &= ~(1ULL << (id % ENTITY_BIT_SIZE));
        node = node->next;
    }
    return true;
}
#include "core/CDS/details/hash.h"
bool system_manager_entity_signature_changed(SystemManager* manager, EntityID id, Mask64 entitySignature){
    MapNode* node = manager->mSystems.start;
    while(node){
        System* s = node->value;
        Mask64 systemSignature = s->targetsSignature;
        if((systemSignature & entitySignature) == systemSignature){
            s->targets[id / ENTITY_BIT_SIZE] |= (1ULL << (id % ENTITY_BIT_SIZE));
        }else{
            s->targets[id / ENTITY_BIT_SIZE] &= ~(1ULL << (id % ENTITY_BIT_SIZE));
        }
        node = node->next;
    }
    return true;
}

bool system_manager_start_systems(SystemManager* manager){
    //TODO : implement system priority order start
    MapNode* sysNode = manager->mSystems.start;
    while(sysNode){
        System* s = sysNode->value;
        if(s->start != null || s->start != 0) s->start(s->self);
        sysNode = sysNode->next;
    }
    return true;
}

bool system_manager_update_systems(SystemManager* manager, FrameInfo* frame){
    //TODO : implement system priority order update
    MapNode* sysNode = manager->mSystems.start;



    while(sysNode){
        //for each registered system
        System* s = sysNode->value;
        if(s->preUpdate != null || s->preUpdate != 0) s->preUpdate(s->self, frame);

        // DEBUG("num of entities targeted by system is : %d", s->targets.length)
        u32 length=0, updated=0;
        for (u32 i = 0; i < ENTITY_U64_SIZE+1; ++i) {
            u64 bitset = s->targets[i];
            u64 entityIndex = i * ENTITY_BIT_SIZE;

            while (bitset != 0) {
                if (bitset & 1) {
                    if(s->update != null || s->update != 0) s->update(s->self, entityIndex, frame);
                    updated++;
                }
                bitset >>= 1;
                entityIndex++;
            }
            length++;
        }

        // TRACE("system [%s] updated %d entities out of %d.",s->systemName, updated, length);

        if(s->AfterUpdate != null || s->AfterUpdate != 0) s->AfterUpdate(s->self, frame);

        sysNode = sysNode->next;
    }
    return true;
}

bool system_manager_shutdown_systems(SystemManager* manager){
    //TODO : implement system priority order shutdown
    MapNode* sysNode = manager->mSystems.start;

    while(sysNode){
        System* s = sysNode->value;
        if(s->shutdown != null || s->shutdown != 0) s->shutdown(s->self);
        free(s->self);
        sysNode = sysNode->next;
    }
    return true;
}