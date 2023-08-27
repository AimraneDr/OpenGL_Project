#ifndef MESH_RENDERER_COMPONENT_H
#define MESH_RENDERER_COMPONENT_H

#include "component_types.h"
#include "engine_types.h"

DLL_EXPORT MeshRenderer mesh_renderer_create(Mesh* m);
DLL_EXPORT bool mesh_renderer_draw_mesh(Shader* shader, MeshRenderer* m, Transform transform, Camera* cam);
DLL_EXPORT bool mesh_renderer_destroy(MeshRenderer* m);

#endif /* MESH_RENDERER_COMPONENT_H */