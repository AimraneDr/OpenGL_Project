#ifndef CDS_COLLECTION_TYPES_H
#define CDS_COLLECTION_TYPES_H

#include "defines.h"

#define _S_MAP_SIZE   100
#define _M_MAP_SIZE   1000
#define _L_MAP_SIZE   10000
#define _XL_MAP_SIZE  100000
#define _XXL_MAP_SIZE 1000000

typedef enum KeyType{
    MAP_KEY_TYPE_U64,
    MAP_KEY_TYPE_U32,
    MAP_KEY_TYPE_U16,
    MAP_KEY_TYPE_U8,
    MAP_KEY_TYPE_S64,
    MAP_KEY_TYPE_S32,
    MAP_KEY_TYPE_S16,
    MAP_KEY_TYPE_S8,
    MAP_KEY_TYPE_F64,
    MAP_KEY_TYPE_F32,
    MAP_KEY_TYPE_STRING
    //add support for costume structures like vec3, vec2, mat3, mat4
}KeyType;

typedef struct MapNode{
    void* key;
    void* value;
    /// @brief points to the previous inserted node
    struct MapNode* prev;
    /// @brief points to the next inserted node
    struct MapNode* next;
    bool isAlive;
}MapNode;

typedef struct SmallUnorderedMap{
    MapNode data[_S_MAP_SIZE];
    /// @brief first inseerted node
    MapNode* start;
    /// @brief last inserted node
    MapNode* end;
    /// @brief active elements
    u32 length;
    KeyType type;
}SmallUnorderedMap;
typedef struct MediumUnorderedMap{
    MapNode data[_M_MAP_SIZE];
    /// @brief first inseerted node
    MapNode* start;
    /// @brief last inserted node
    MapNode* end;
    /// @brief active elements
    u32 length;
    KeyType type;
}MediumUnorderedMap;
typedef struct LargeUnorderedMap{
    MapNode data[_L_MAP_SIZE];
    /// @brief first inseerted node
    MapNode* start;
    /// @brief last inserted node
    MapNode* end;
    /// @brief active elements
    u32 length;
    KeyType type;
}LargeUnorderedMap;
typedef struct XLargeUnorderedMap{
    MapNode data[_XL_MAP_SIZE];
    /// @brief first inseerted node
    MapNode* start;
    /// @brief last inserted node
    MapNode* end;
    /// @brief active elements
    u32 length;
    KeyType type;
}XLargeUnorderedMap;
typedef struct XXLargeUnorderedMap{
    MapNode data[_XXL_MAP_SIZE];
    /// @brief first inseerted node
    MapNode* start;
    /// @brief last inserted node
    MapNode* end;
    /// @brief active elements
    u32 length;
    KeyType type;
}XXLargeUnorderedMap;

typedef struct UnorderedMap{
    MapNode* data;
    u32 maxSize;
    /// @brief first inseerted node
    MapNode* start;
    /// @brief last inserted node
    MapNode* end;
    /// @brief active elements
    u32 length;
    KeyType type;
}UnorderedMap;

#endif /* CDS_COLLECTION_TYPES_H */