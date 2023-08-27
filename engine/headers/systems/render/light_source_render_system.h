/**
 * @file light_source_render_system.h
 * @author your name (aimranedr@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SYSTEM_LIGHT_SOURCE_RENDERER_H
#define SYSTEM_LIGHT_SOURCE_RENDERER_H

#include "defines.h"
#include "render/render_types.h"
#include "ecs/ecs_defines.h"
#include "systems/render/mainMeshRendererSystem.h"

typedef struct LightSourceRenderSystem{
    RenderPipeline pipeline;
    MainMeshRendererSystem* rSys;
}LightSourceRenderSystem;


bool light_source_render_system_start(void* self);

bool light_source_render_system_pre_update(void* self, void* frame_info);
bool light_source_render_system_update(void* self, EntityID entity, void* frame_info);
bool light_source_render_system_after_update(void* self, void* frame_info);

bool light_source_render_system_shutdown(void* self);

#endif /* SYSTEM_LIGHT_SOURCE_RENDERER_H */