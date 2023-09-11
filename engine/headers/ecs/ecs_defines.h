#ifndef ECS_DEFINES_H
#define ECS_DEFINES_H

#include "core/CDS/collection_types.h"

#define MAX_COMPONENT_TYPES (sizeof(Mask64) * 8)
#define ENTITY_BIT_COUNT 64
#define MAX_ENTITIES 10000
#define INVALID_ENTITY_ID -1
#define INVALID_COMPONENT_TYPE 64
#define EMPTY_COMPONENT_MASK 0
#define ENTITY_U64_MAPS_COUNT ((MAX_ENTITIES % ENTITY_BIT_COUNT == 0) ? (MAX_ENTITIES / ENTITY_BIT_COUNT) : ((MAX_ENTITIES / ENTITY_BIT_COUNT) + 1))

typedef unsigned long long  ID;
typedef unsigned long long  Mask64;
typedef unsigned long long  EntityID;

//index into a componentArray's array
typedef unsigned char       ComponentType;


typedef struct ComponentArray
{
    char* name;
    ComponentType type;
    u32 componentSize;
    /// @brief acccess component by Entityid
    void* components[MAX_ENTITIES];
    
    u32 aliveComponentsCount;
} ComponentArray;

typedef struct EntityManager{
    /// @brief array of component masks, the index is the entity id
    Mask64 entitiesComponentMasks[MAX_ENTITIES];

    /// @brief dynamic array of EntityID
    EntityID* freeIds;
    u64 activeEntitiesCount;
}EntityManager;

typedef struct ComponentManager{
    /// @brief indexed by a ComponentType
    ComponentArray* ComponentsArrays[MAX_COMPONENT_TYPES];
    
    /// @brief count of the total registered components 
    u8 componentsCount;
}ComponentManager;


typedef bool (*SystemStartFunc)(void*);
typedef bool (*SystemPreUpdateFunc)(void*, void*);
typedef bool (*SystemUpdateFunc)(void*, EntityID, void*);
typedef bool (*SystemAfterUpdateFunc)(void*, void*);
typedef bool (*SystemShutdownFunc)(void*);

typedef struct SystemRegisterInfo{
    /// @brief use TYPE_NAME() from 'defines.h' to data type name as string
    char* systemName;
    u32 systemSize;
    void* self;
    SystemStartFunc start;
    SystemPreUpdateFunc preUpdate;
    SystemUpdateFunc update;
    SystemAfterUpdateFunc AfterUpdate;
    SystemShutdownFunc shutdown;
}SystemRegisterInfo;

typedef struct System{
    //to be removed
    char* systemName;
    /// @brief components dependency bitmap, when updated changes will apply in the next frame
    Mask64 targetsSignature;

    /// @brief array of bool tells if an entity is targett
    //change to tree
    u64* targets;

    /// @brief reference to the registered system struct
    void* self;

    //here goes all function that a system should be able to perform

    /// @brief runs at the start of the system.
    /// @param (void*)self to be cast back to the wanted system struct type .
    /// @param (UnorderedMap)targets holds all the entities the system is targeting.
    /// @return true for success, false for failed.
    SystemStartFunc start;

    /// @brief runs first, once each frame.
    /// @param (void*)self to be cast back to the wanted system struct type .
    /// @param (EntityID)entity the entity targeted.
    /// @param (void*)frame object contains all info needed about a frame.
    /// @return true for success, false for failed.
    SystemPreUpdateFunc preUpdate;

    /// @brief runs for each targeted entity on each frame after
    /// @param (void*)self to be cast back to the wanted system struct type .
    /// @param (UnorderedMap)targets holds all the entities the system is targeting.
    /// @param ((void*)frame object contains all info needed about a frame.
    /// @return true for success, false for failed.
    SystemUpdateFunc update;

    /// @brief runs last, once each frame,
    /// @param (void*)self to be cast back to the wanted system struct type .
    /// @param (EntityID)entity the entity targeted.
    /// @param (void*)frame object contains all info needed about a frame.
    /// @return true for success, false for failed.
    SystemAfterUpdateFunc AfterUpdate;

    /// @brief runs at the shutdown of the system.
    /// @param (void*)self to be cast back to the wanted system struct type. 
    /// @param (UnorderedMap)targets holds all the entities the system is targeting.
    /// @return true for success, false for failed.
    SystemShutdownFunc shutdown;
}System;

typedef struct SystemManager{
    /// @brief maps a name to a system
    UnorderedMap mSystems;
    u32 systemsCount;
}SystemManager;

typedef struct ECSManager{
    EntityManager entityManager;
    ComponentManager componentManager;
    SystemManager systemManager;
}ECSManager;

#endif /* ECS_DEFINES_H */