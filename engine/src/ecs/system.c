#include "ecs/system.h"

#include "core/logger.h"
#include "core/CDS/strings.h"
#include <stdlib.h>
#include <string.h>

System* system_create_new_system(SystemRegisterInfo info){
    //ENTITY_U64_MAPS_COUNT
    System* out = (System*)malloc(sizeof(System));
    out->systemName = (char*)malloc(stringLength(info.systemName));
    stringCopy(out->systemName, info.systemName);
    out->self = info.self;
    out->start = info.start;
    out->preUpdate = info.preUpdate;
    out->update = info.update;
    out->AfterUpdate = info.AfterUpdate;
    out->shutdown = info.shutdown;

    out->targets = (u64*)malloc(sizeof(u64) * ENTITY_U64_MAPS_COUNT);
    if(out->targets == null){
        FATAL("Failed to allocate memory for system's target list, aborting the program");
        return false;
    }
    memset(out->targets, 0, sizeof(u64) * ENTITY_U64_MAPS_COUNT);

    return out;
}

bool system_add_entity_to_targets_list(System* s, EntityID e){
    u32 bitMaskIndex = e / ENTITY_BIT_COUNT;
    u32 entityIndexInBitMask = e % ENTITY_BIT_COUNT;

    s->targets[bitMaskIndex] |= (1ULL << entityIndexInBitMask);
    return true;
}

bool system_is_targetting_entity(System* s, EntityID e){
    u32 bitMaskIndex = e / ENTITY_BIT_COUNT;
    u32 entityIndexInBitMask = e % ENTITY_BIT_COUNT;

    return ((s->targets[bitMaskIndex] & (1ULL << entityIndexInBitMask)) >> entityIndexInBitMask) == 1 ? true : false;
}

bool system_remove_entity_from_targets_list(System* s, EntityID e){
    u32 bitMaskIndex = e / ENTITY_BIT_COUNT;
    u32 entityIndexInBitMask = e % ENTITY_BIT_COUNT;

    s->targets[bitMaskIndex] &= ~(1ULL << entityIndexInBitMask);
    return true;
}

bool system_clear_targets_list(System* s){
    memset(s->targets, 0, sizeof(u64) * ENTITY_U64_MAPS_COUNT);
    free(s->targets);
    s->targets = null;
    return true;
}

bool system_clean_up(System* s){
    // move to be handled by the allocator
    free(s->self);
    //
    free(s->systemName);
    s->systemName = 0;
    s->self = 0;
    s->targetsSignature = 0;
    s->start = 0;
    s->preUpdate = 0;
    s->AfterUpdate = 0;
    s->update = 0;
    s->shutdown = 0;
    system_clear_targets_list(s);
    free(s);
    s = null;
    return true;
}