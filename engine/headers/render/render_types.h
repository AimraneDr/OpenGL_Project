#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

#include "defines.h"
#include "math/math_defines.h"

typedef enum RenderAPI{
    OPEN_GL_4,
    VULKAN,
    DIRECT_X
}RenderAPI;

typedef struct RenderSystemInfo{
    RenderAPI api;
}RenderSystemInfo;

typedef struct VertexArrayObject{
    u32 id;
}VAO;

typedef struct VertexBufferObject{
    u32 id;
}VBO;

typedef struct IndexBufferObject{
    u32 id;
}IBO;

typedef struct UniformBufferObject{
    u32 id;
}UBO;

typedef struct Texture{
    u32 id;
    u32 uniformID;
}Texture;

typedef struct Shader{
    u32 id;
    char* name;

    //unigorm buffers
    UBO* UBOs;
    u32 UBOs_count;
}Shader;


typedef struct RenderPipeline{

    //move to material
    Texture tex;

    //list of shaders
    Shader* shaderPrograms;
    u32 shadersCount;
}RenderPipeline;

typedef struct redererState{
    RenderPipeline* pipelines;

    RenderSystemInfo info;
}rendererState;

typedef struct Vertex{
    Vec3 position;
    Vec3 normal;
    Vec3 color;
    Vec2 uv;
}Vertex;

typedef struct Face{
    Vertex vertices[3];
    u32 material_index;
}Face;


typedef struct Material {
    Texture albedoMap;      // Albedo (color) texture
    Texture normalMap;      // Normal map texture
    Texture roughnessMap;   // Roughness map texture
    Texture metalnessMap;   // Metalness map texture
    Texture aoMap;          // Ambient occlusion map texture

    Vec3 albedoColor;       // Base color (albedo)
    f32 roughness;          // Surface roughness
    f32 metalness;          // Metalness
    f32 ao;                 // Ambient occlusion

    Texture specularMap;    // Specular map texture (might not be used in PBR)
    f32 specularIntensity;  // Intensity of specular reflections
    f32 shininess;          // Shininess (might not be used in PBR)

    u32 id;                 // Material identifier
} Material;


typedef struct Mesh{
    Vertex* vertices;
    u32* indices;
    Face* faces;

    Material* materials;

    u32 numVertices, numIndices;

}Mesh;
#endif /* RENDER_TYPES_H */