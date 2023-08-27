#include "core/CDS/unoredered_map.h"

#include "core/CDS/details/hash.h"
#include <stdlib.h>

#define DUMMY_DEFINE 0

u64 hash(void* key, u32 max, KeyType t){
    u64 v = 0;
    switch (t)
    {
        case MAP_KEY_TYPE_U64:
        case MAP_KEY_TYPE_U32:
        case MAP_KEY_TYPE_U16:
        case MAP_KEY_TYPE_U8:
            v = hash_unsigned((u64)key,max);
            break;
        case MAP_KEY_TYPE_S64:
        case MAP_KEY_TYPE_S32:
        case MAP_KEY_TYPE_S16:
        case MAP_KEY_TYPE_S8:
            v = hash_signed((s64)key,max);
            break;
        case MAP_KEY_TYPE_F64:
            v = hash_f64(*((f64*)key), max);
            break;
        case MAP_KEY_TYPE_F32:
            v = hash_f32(*((f32*)key), max);
            break;
        case MAP_KEY_TYPE_STRING:
            v = hash_string((char*)key,max);
            break;
        default:
            break;
    }
    return v;
}

//Costume Sized
bool unordered_map_create(UnorderedMap* out_map, u32 max_size, KeyType type){
    out_map->maxSize = max_size;
    out_map->data = (MapNode*)malloc(sizeof(MapNode) * out_map->maxSize);
    if(out_map->data == null) return false;
    out_map->length = 0;
    for(u32 i=0; i< out_map->maxSize; i++){
        MapNode mnode = {
            .key = 0,
            .next = 0,
            .prev = 0,
            .value = 0,
            .isAlive = false
        };
        out_map->data[i] = mnode;
    }
    out_map->end = 0;
    out_map->type = type;
    out_map->start = 0;
    return true;
}

bool unordered_map_resize(UnorderedMap* map, u32 new_max_size){
    if(map->length > new_max_size) {
        return false;
    }
     
    UnorderedMap new_map = {};
    new_map.maxSize = new_max_size;
    new_map.data = (MapNode*)malloc(sizeof(MapNode) * new_map.maxSize);
    if(new_map.data == null) return false;
    new_map.length = 0;
    for(u32 i=0; i< new_map.maxSize; i++){
        MapNode mnode = {
            .key = 0,
            .next = 0,
            .prev = 0,
            .value = 0,
            .isAlive = false
        };
        new_map.data[i] = mnode;
    }
    new_map.end = 0;
    new_map.start = 0;

    MapNode* oldNode = map->start;
    while(oldNode){
        void* key = oldNode->key;
        u64 newIndex = hash(key, new_max_size, map->type);
        new_map.data[newIndex].key = key;
        new_map.data[newIndex].next = oldNode->next;
        new_map.data[newIndex].prev = oldNode->prev;
        new_map.data[newIndex].value = oldNode->value;
        new_map.data[newIndex].isAlive = true;

        oldNode = oldNode->next;
    }

    unordered_map_clean(map);
    *map = new_map;
    return true;
}
#include "core/logger.h"
bool unordered_map_set(UnorderedMap* map, void* key, void* value){
    u64 index = hash(key,map->maxSize, map->type);
    if(map->data[index].isAlive){
        //slot already live
        //update key value
        map->data[index].key = key;
        map->data[index].value = value;
    }else{
        MapNode mnode = {
            .key = key,
            .prev = 0,
            .next = 0,
            .value = value,
            .isAlive = true
        };
        map->data[index] = mnode;
        if(map->length == 0){
            map->start = &map->data[index];
            map->end = &map->data[index];
        }else{
            map->data[index].prev = map->end;
            map->end->next = &map->data[index];
            map->end = &map->data[index];
        }
        map->length++;
    }
    return true;
}

bool unordered_map_unset(UnorderedMap* map, void* key){
    if(map->length <= 0) {
        return false;
    }
    MapNode mnode = {
        .key = 0,
        .prev = 0,
        .next = 0,
        .value = 0,
        .isAlive = false
    };
    u64 index = hash(key,map->maxSize, map->type);
    if(map->length == 1){
        map->start = 0;
        map->end = 0;
    }else{
        if(map->data[index].next == 0 && map->data[index].prev != 0){
            //last element
            map->end = map->data[index].prev; 
            map->data[index].prev->next = 0;
        }else if(map->data[index].prev == 0 && map->data[index].next != 0){
            //first element
            map->start = map->data[index].next; 
            map->data[index].next->prev = 0;
        }else{
            MapNode * n = map->data[index].next;
            MapNode * p = map->data[index].prev;
            n->prev = p;
            p->next = n;
        }
    }
    map->data[index] = mnode;
    map->length--;
    return true;
}


bool unordered_map_is_set(UnorderedMap* map, void* key){
    if(map->length == 0) {
        return false;
    }
    u64 index = hash(key,map->maxSize, map->type);
    return map->data[index].isAlive;
}

void* unordered_map_get(UnorderedMap* map, void* key){
    if(map->length <= 0) {
        return null;
    }
    u64 index = hash(key,map->maxSize, map->type);
    if(!map->data[index].isAlive) return null;
    return map->data[index].value;
}

bool unordered_map_clean(UnorderedMap* map){
    free(map->data);
    map->data = 0;
    map->length = 0;
    map->end = 0;
    map->start = 0;
    return true;
}