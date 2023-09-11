#ifndef CDS_MAP_H
#define CDS_MAP_H

#include "defines.h"
#include "collection_types.h"


//SMALL
bool unordered_map_s_create(SmallUnorderedMap* out_map, KeyType type);
bool unordered_map_s_set(SmallUnorderedMap* map, void* key, void* value);
bool unordered_map_s_unset(SmallUnorderedMap* map, void* key);
void* unordered_map_s_get(SmallUnorderedMap* map, void* key);
bool unordered_map_s_clean(SmallUnorderedMap* out_map);

//Medium
bool unordered_map_m_create(MediumUnorderedMap* out_map, KeyType type);
bool unordered_map_m_set(MediumUnorderedMap* map, void* key, void* value);
bool unordered_map_m_unset(MediumUnorderedMap* map, void* key);
void* unordered_map_m_get(MediumUnorderedMap* map, void* key);
bool unordered_map_m_clean(MediumUnorderedMap* out_map);

//Large
bool unordered_map_l_create(LargeUnorderedMap* out_map, KeyType type);
bool unordered_map_l_set(LargeUnorderedMap* map, void* key, void* value);
bool unordered_map_l_unset(LargeUnorderedMap* map, void* key);
void* unordered_map_l_get(LargeUnorderedMap* map, void* key);
bool unordered_map_l_clean(LargeUnorderedMap* out_map);

//X-Large
bool unordered_map_xl_create(XLargeUnorderedMap* out_map, KeyType type);
bool unordered_map_xl_set(XLargeUnorderedMap* map, void* key, void* value);
bool unordered_map_xl_unset(XLargeUnorderedMap* map, void* key);
void* unordered_map_xl_get(XLargeUnorderedMap* map, void* key);
bool unordered_map_xl_clean(XLargeUnorderedMap* out_map);

//XX-Large
bool unordered_map_xxl_create(XXLargeUnorderedMap* out_map, KeyType type);
bool unordered_map_xxl_set(XXLargeUnorderedMap* map, void* key, void* value);
bool unordered_map_xxl_unset(XXLargeUnorderedMap* map, void* key);
void* unordered_map_xxl_get(XXLargeUnorderedMap* map, void* key);
bool unordered_map_xxl_clean(XXLargeUnorderedMap* out_map);

//Costume Sized
bool unordered_map_create(UnorderedMap* out_map, u32 max_size, KeyType type);
bool unordered_map_resize(UnorderedMap* map, u32 new_max_size);
bool unordered_map_set(UnorderedMap* map, void* key, void* value);
bool unordered_map_unset(UnorderedMap* map, void* key);
bool unordered_map_is_set(UnorderedMap* map, void* key);
/// @brief 
/// @param map 
/// @param key 
/// @return value pointed by key, null if not exists 
void* unordered_map_get(UnorderedMap* map, void* key);
bool unordered_map_clean(UnorderedMap* out_map);

#endif /* CDS_MAP_H */