/**
 * @file light_source_render_system.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "systems/render/light_source_render_system.h"

#include "engine_types.h"
#include "core/files.h"
#include "core/logger.h"
#include "core/CDS/dynamic_array.h"
#include "render/render_types.h"
#include "render/shader.h"
#include "component_types.h"
#include "components/mesh_renderer.h"
#include "components/transform.h"
#include "ecs/ecs_manager.h"
#include "core/CDS/strings.h"
#include "math/math.h"
#include "render/camera.h"
#include "render/vertex_array.h"

#include <glad/glad.h>
#include <stdlib.h>

bool light_source_render_system_start(void* self){
    LightSourceRenderSystem* sys = (LightSourceRenderSystem*) self;

    sys->pipeline.shadersCount = 2;
    sys->pipeline.shaderPrograms = (Shader*)malloc(sizeof(Shader) * sys->pipeline.shadersCount);
    
    shaderConfigInfo configInfo[2];
    configInfo[0].vertexShaderFilename = file_join_path(ENGINE_RESOURCES_PATH, "shaders/light_sources/pointlight.vert");
    configInfo[0].fragmentShaderFilename = file_join_path(ENGINE_RESOURCES_PATH, "shaders/light_sources/pointlight.frag");
    configInfo[0].geometryShaderFilename = 0;
    configInfo[1].vertexShaderFilename = file_join_path(ENGINE_RESOURCES_PATH, "shaders/light_sources/meshlightsource.vert");
    configInfo[1].fragmentShaderFilename = file_join_path(ENGINE_RESOURCES_PATH, "shaders/light_sources/meshlightsource.frag");
    configInfo[1].geometryShaderFilename = 0;
    sys->pipeline.shaderPrograms[0].name = "pointLightShader";
    sys->pipeline.shaderPrograms[1].name = "meshLightSourceShader";
    shader_create(&configInfo[0], &sys->pipeline.shaderPrograms[0]);
    shader_create(&configInfo[1], &sys->pipeline.shaderPrograms[1]);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    return true;
}

// bool light_source_render_system_pre_update(void* self, void* frame_info){
//     LightSourceRenderSystem* sys = (LightSourceRenderSystem*) self;
//     FrameInfo* frame = (FrameInfo*) frame_info;

//     return true;
// }

/**
 * @brief 
 * 
 * @param self 
 * @param entity 
 * @param frame_info 
 * @return true 
 * @return false 
 * @todo change return type to RESULT_MSG
 */
bool light_source_render_system_update(void* self, EntityID entity, void* frame_info){
    LightSourceRenderSystem* sys = (LightSourceRenderSystem*) self;
    FrameInfo* frame = (FrameInfo*) frame_info;

    LightSource* light = (LightSource*)ecs_manager_get_component(0, entity, TYPE_NAME(LightSource));
    Transform* transform = (Transform*)ecs_manager_get_component(0, entity, TYPE_NAME(Transform));
    if(light == null) return false;
    dynamic_array_push(sys->rSys->lightsEIds, entity);

    //render nothing if the light is directional
    if(light->type == LIGHT_SOURCE_TYPE_DIRECTIONAL) return true;

    Shader* selected_shader = 0;
    if(light->type == LIGHT_SOURCE_TYPE_MESH){
        selected_shader = &sys->pipeline.shaderPrograms[1];
        shader_use(selected_shader);
        
        // glUniform1f(glGetUniformLocation(sys->pipeline.shaderPrograms[1].id, "light_radius"), light->radius);
        // glUniform1f(glGetUniformLocation(sys->pipeline.shaderPrograms[1].id, "light_half_angle"), light->halfAngle);
        glUniform4fv(glGetUniformLocation(selected_shader->id, "light_color"), 1, light->color.elements);
    
    }else if(light->type == LIGHT_SOURCE_TYPE_POINT){
        selected_shader = &sys->pipeline.shaderPrograms[0];
        shader_use(selected_shader);
        transform->rotation = vec3(0,0,0);
        glUniformMatrix4fv(glGetUniformLocation(selected_shader->id, "view"),1,GL_FALSE, frame->camera->viewMatrix.elements);
        glUniform3fv(glGetUniformLocation(selected_shader->id, "light_pos"), 1, transform->position.elements);

        glUniform4fv(glGetUniformLocation(selected_shader->id, "light_color"), 1, light->color.elements);
    }
    mesh_renderer_draw_mesh(selected_shader, &light->__mr__, *transform, frame->camera);

    return true;
}

// bool light_source_render_system_after_update(void* self, void* frame_info){
//     LightSourceRenderSystem* sys = (LightSourceRenderSystem*) self;
//     FrameInfo* frame = (FrameInfo*) frame_info;

//     return true;
// }

bool light_source_render_system_shutdown(void* self){
    LightSourceRenderSystem* sys = (LightSourceRenderSystem*) self;
    for(u32 i=0; i < sys->pipeline.shadersCount; i++){
        shader_delete(&sys->pipeline.shaderPrograms[i]);
    }
    free(sys->pipeline.shaderPrograms);
    sys->pipeline.shadersCount = 0;
    return true;
}