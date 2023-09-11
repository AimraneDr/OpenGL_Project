#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include "defines.h"
#include "ecs/ecs_defines.h"
#include "math/math_defines.h"
#include "render/render_types.h"

/// @brief Transform Component
typedef struct transform{
    EntityID eID;
    
    //world transformation
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
}Transform;

/// @brief Mesh Renderer Component
typedef struct meshRenderer{
    Mesh mesh;

    VBO VBO;
    IBO IBO;
    VAO VAO;

}MeshRenderer;

typedef enum LightSourceType{
    LIGHT_SOURCE_TYPE_POINT,
    LIGHT_SOURCE_TYPE_MESH,
    LIGHT_SOURCE_TYPE_SPOT,
    LIGHT_SOURCE_TYPE_AREA,
    LIGHT_SOURCE_TYPE_DIRECTIONAL
}LightSourceType;

typedef struct lightSource{
    const LightSourceType type;
    Vec4 color;
    f32 radius;
    f32 halfAngle; 

    bool castShadows;

    MeshRenderer __mr__;
}LightSource;


#endif /* COMPONENT_TYPES_H */