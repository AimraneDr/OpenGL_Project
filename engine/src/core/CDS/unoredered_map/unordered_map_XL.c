#include "core/CDS/unoredered_map.h"

#include "core/logger.h"
#include "core/CDS/details/hash.h"


u64 hashXL(void* key, KeyType t){
    u64 v = 0;
    switch (t)
    {
        case MAP_KEY_TYPE_U64:
        case MAP_KEY_TYPE_U32:
        case MAP_KEY_TYPE_U16:
        case MAP_KEY_TYPE_U8:
            v = hash_unsigned((u64)key,_XL_MAP_SIZE);
            break;
        case MAP_KEY_TYPE_S64:
        case MAP_KEY_TYPE_S32:
        case MAP_KEY_TYPE_S16:
        case MAP_KEY_TYPE_S8:
            v = hash_signed((s64)key,_XL_MAP_SIZE);
            break;
        case MAP_KEY_TYPE_F64:
            v = hash_f64(*((f64*)key),_XL_MAP_SIZE);
            break;
        case MAP_KEY_TYPE_F32:
            v = hash_f32(*((f32*)key),_XL_MAP_SIZE);
            break;
        case MAP_KEY_TYPE_STRING:
            v = hash_string((char*)key,_XL_MAP_SIZE);
            break;
        default:
            break;
    }
    return v;
}

bool unordered_map_xl_create(XLargeUnorderedMap* out_map, KeyType type){
    XLargeUnorderedMap map = {};
    map.length = 0;
    for(u32 i=0; i< _XL_MAP_SIZE; i++){
        MapNode mnode = {
            .key = 0,
            .next = 0,
            .prev = 0,
            .value = 0,
            .isAlive = false
        };
        map.data[i] = mnode;
    }
    map.end = 0;
    map.start = 0;
    map.type =type;
    *out_map = map;
    return true;
}

bool unordered_map_xl_set(XLargeUnorderedMap* map, void* key, void* value){
    u64 index = hashXL(key,map->type);
    if(map->data[index].isAlive){
        //slot already live
        //update key value
        map->data[index].key = key;
        map->data[index].value = value;
    }else{
        MapNode mnode = {
            .key = key,
            .prev = map->end,
            .next = 0,
            .value = value,
            .isAlive = true
        };
        map->data[index] = mnode;
        if(map->length == 0){
            map->start = &map->data[index];
            map->end = &map->data[index];
        }
        map->end->next = &map->data[index];
        map->end = &map->data[index];
        map->length++;
    }
    return true;
}
bool unordered_map_xl_unset(XLargeUnorderedMap* map, void* key){
    if(map->length <= 0) {
        ERR("attempt to remove an element from an empty map")
        return false;
    }
    MapNode mnode = {
        .key = 0,
        .prev = 0,
        .next = 0,
        .value = 0,
        .isAlive = false
    };
    u64 index = hashXL(key,map->type);
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

void* unordered_map_xl_get(XLargeUnorderedMap* map, void* key){
    if(map->length <= 0) {
        ERR("requesting value from an empty map")
        return null;
    }
    u64 index = hashXL(key,map->type);
    MapNode n = map->data[index];
    if(!n.isAlive) return null;
    return map->data[index].value;
}

bool unordered_map_xl_clean(XLargeUnorderedMap* map){
    for(u32 i=0; i< _XL_MAP_SIZE; i++){
        MapNode mnode = {
            .key = 0,
            .next = 0,
            .value = 0,
            .isAlive = false
        };
        map->data[i] = mnode;
    }
    map->length = 0;
    return true;
}