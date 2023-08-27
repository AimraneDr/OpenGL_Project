#include "systems/render/mainMeshRendererSystem.h"

#include <glad/glad.h>

#include "engine_types.h"
#include "ecs/ecs_manager.h"

#include "render/shader.h"
#include "render/texture.h"
#include "render/vertex_array.h"
#include "render/uniform_buffer.h"
#include "render/camera.h"

#include "core/files.h"
#include "core/logger.h"
#include "core/CDS/dynamic_array.h"

#include "math/math.h"

#include "component_types.h"
#include "components/mesh_renderer.h"

// TODO : move to another place
typedef struct PointLightSourceUniformInterface{
    Vec4 color;
    Vec3 position;
    f32 radius;
}PointLightSourceUniformInterface;
typedef struct DirectionalLightSourceUniformInterface{
    Vec4 color;
    Vec3 direction;
}DirectionalLightSourceUniformInterface;

const u32 MAX_POINT_LIGHTS = 20;
const u32 MAX_DIRECTIONAL_LIGHTS = 10;

bool main_mesh_renderer_system_start(void* self){
    MainMeshRendererSystem* sys = (MainMeshRendererSystem*) self;

    //temp : introduce component based search algo to avoid storing lights in the system
    sys->lightsEIds = dynamic_array_create(EntityID);


    sys->pipeline.shadersCount = 1;
    sys->pipeline.shaderPrograms = (Shader*)malloc(sizeof(Shader) * sys->pipeline.shadersCount);
    // TODO : rename the shader files to a more convinient name
    shaderConfigInfo configInfo = {};
    configInfo.vertexShaderFilename = file_join_path(ENGINE_RESOURCES_PATH,"shaders/default.vert");
    configInfo.fragmentShaderFilename = file_join_path(ENGINE_RESOURCES_PATH,"shaders/default.frag");
    configInfo.geometryShaderFilename = 0;
    configInfo.UBOs_initial_num = 1;

    shader_create(&configInfo, &sys->pipeline.shaderPrograms[0]);
    shader_use(&sys->pipeline.shaderPrograms[0]);

    //create the uniform buffer holding the light sources
    /// 1 - directional lights

    ubo_create(0, sizeof(LightSource) * MAX_DIRECTIONAL_LIGHTS, &sys->pipeline.shaderPrograms->UBOs[0]);

    /// 2 - point lights
    ubo_create(1, sizeof(LightSource) * MAX_POINT_LIGHTS, &sys->pipeline.shaderPrograms->UBOs[1]);

    //temp => move to material
    //texture
    texture_create(
        file_join_path(ENGINE_RESOURCES_PATH, "images/brick.png"),
        &sys->pipeline.shaderPrograms[0],
        &sys->pipeline.tex
    );    

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    return true;
}

bool main_mesh_renderer_system_pre_update(void* self, void* frame_info){
    MainMeshRendererSystem* sys = (MainMeshRendererSystem*) self;
    FrameInfo* frame = (FrameInfo*) frame_info;
   
    // glClearColor(0.07f,.13f,.17f,1.f);
    shader_use(&sys->pipeline.shaderPrograms[0]);
    
    //update the uniform buffers
    //TODO : optimize light sources passing
    u32 total_lights = dynamic_array_length(sys->lightsEIds);
    u32 pointLightCurrentIndex = 0;
    u32 directionalLightCurrentIndex = 0;
    PointLightSourceUniformInterface pointLts[MAX_POINT_LIGHTS] = {};
    DirectionalLightSourceUniformInterface dirctionalLts[MAX_DIRECTIONAL_LIGHTS] = {};
    for(u32 i =0; i < total_lights; i++){
        Transform* t = (Transform*)ecs_manager_get_component(0,sys->lightsEIds[i], TYPE_NAME(Transform));
        LightSource* l = (LightSource*)ecs_manager_get_component(0,sys->lightsEIds[i], TYPE_NAME(LightSource));
        if(l->type == LIGHT_SOURCE_TYPE_POINT || l->type == LIGHT_SOURCE_TYPE_MESH){
            pointLts[pointLightCurrentIndex].color = l->color;
            pointLts[pointLightCurrentIndex].position = t->position;
            pointLts[pointLightCurrentIndex].radius = l->radius;
            pointLightCurrentIndex++;
        }else if(l->type == LIGHT_SOURCE_TYPE_DIRECTIONAL){
            dirctionalLts[directionalLightCurrentIndex].color = l->color;
            dirctionalLts[directionalLightCurrentIndex].direction = t->rotation;
            directionalLightCurrentIndex++;
        }
    }

    ubo_bind(&sys->pipeline.shaderPrograms->UBOs[0]);
    PointLightSourceUniformInterface* pointLightData = (PointLightSourceUniformInterface*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(pointLightData, pointLts, sizeof(PointLightSourceUniformInterface) * pointLightCurrentIndex);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    ubo_bind(&sys->pipeline.shaderPrograms->UBOs[1]);
    DirectionalLightSourceUniformInterface* directionalLightData = (DirectionalLightSourceUniformInterface*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(directionalLightData, dirctionalLts, sizeof(DirectionalLightSourceUniformInterface) * directionalLightCurrentIndex);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    ubo_unbind();

    glUniform1ui(glGetUniformLocation(sys->pipeline.shaderPrograms[0].id, "point_light_count"), pointLightCurrentIndex + 1);
    glUniform1ui(glGetUniformLocation(sys->pipeline.shaderPrograms[0].id, "directional_light_count"), directionalLightCurrentIndex + 1);
    glUniform3fv(glGetUniformLocation(sys->pipeline.shaderPrograms[0].id, "cameraPos"), 1, frame->camera->transform.position.elements);
    glUniform1f(glGetUniformLocation(sys->pipeline.shaderPrograms[0].id, "directionalLightSpecularIntensity"), .2f);

    //move to update
    texture_bind(&sys->pipeline.tex);
    return true;
}

bool main_mesh_renderer_system_update(void* self, EntityID entity, void* frame_info){
    MainMeshRendererSystem* sys = (MainMeshRendererSystem*) self;
    FrameInfo* frame = (FrameInfo*) frame_info;

    MeshRenderer* meshRnd = (MeshRenderer*)ecs_manager_get_component(0,entity,TYPE_NAME(MeshRenderer));
    Transform* transform = (Transform*)ecs_manager_get_component(0,entity,TYPE_NAME(Transform));

    if(dynamic_array_length(sys->lightsEIds) > 0){

        for(u32 i=0; i < dynamic_array_length(sys->lightsEIds); i++){
            EntityID e = sys->lightsEIds[i];
            LightSource* light = ecs_manager_get_component(0, e, TYPE_NAME(LightSource));
            Transform* lightT = ecs_manager_get_component(0, e, TYPE_NAME(Transform));

        }

    }else{
        //set default directional light
        glUniform4f(
            glGetUniformLocation(sys->pipeline.shaderPrograms[0].id, "DirectionallightColor"),
            0,0,0,1);
        glUniform3f(
            glGetUniformLocation(sys->pipeline.shaderPrograms[0].id, "Directionallight"),
            0,1,0);
    }

    glUniform1i(
        glGetUniformLocation(sys->pipeline.shaderPrograms[0].id, "lights_count"),
        dynamic_array_length(sys->lightsEIds) + 1
    );

    //pass material if mesh has one

    mesh_renderer_draw_mesh(&sys->pipeline.shaderPrograms[0], meshRnd, *transform, frame->camera);

    return true;
}

bool main_mesh_renderer_system_after_update(void* self, void* frame_info){
    MainMeshRendererSystem* sys = (MainMeshRendererSystem*) self;
    FrameInfo* frame = (FrameInfo*) frame_info;

    // TRACE("fps : %f", 1.f / frame->deltaTime);
    dynamic_array_clear(sys->lightsEIds);
    texture_unbind(&sys->pipeline.tex);
    return true;
}


bool main_mesh_renderer_system_shutdown(void* self){
    MainMeshRendererSystem* sys = (MainMeshRendererSystem*) self;
    shader_delete(&sys->pipeline.shaderPrograms[0]);
    texture_delete(&sys->pipeline.tex);
    free(sys->pipeline.shaderPrograms);
    sys->pipeline.shaderPrograms = 0;
    sys->pipeline.shadersCount = 0;
    return true;
}