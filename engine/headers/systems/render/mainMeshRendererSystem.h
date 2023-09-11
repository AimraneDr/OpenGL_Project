#ifndef SYSTEM_MAIN_MESH_RENDERER_H
#define SYSTEM_MAIN_MESH_RENDERER_H

#include "core/CDS/collection_types.h"
#include "render/render_types.h"
#include "ecs/ecs_defines.h"

typedef struct MainMeshRendererSystem{
    RenderPipeline pipeline;

    //temp
    EntityID* lightsEIds;
}MainMeshRendererSystem;


bool main_mesh_renderer_system_start(void* self);

bool main_mesh_renderer_system_pre_update(void* self, void* frame_info);
bool main_mesh_renderer_system_update(void* self, EntityID entity, void* frame_info);
bool main_mesh_renderer_system_after_update(void* self, void* frame_info);

bool main_mesh_renderer_system_shutdown(void* self);

#endif /* SYSTEM_MAIN_MESH_RENDERER_H */