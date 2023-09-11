#include <entry.h>

#include "core/logger.h"
#include "core/files.h"
#include "ecs/ecs_manager.h"
#include "assets_manager/assets_manager.h"
#include "components/transform.h"
#include "components/mesh_renderer.h"
#include "components/light_source.h"
#include "math/math.h"

bool game_start();

bool game_init(GameSetupInfo* info){
    info->window_info.name = "D_Mark_D test game";
    info->window_info.width = 700;
    info->window_info.height = 500;
    info->window_info.x = 0;
    info->window_info.y = 0;
    info->window_info.resizable = false;//non-functional/ te be fixed later
    info->setup = 0;
    info->start = game_start;
    info->update = 0;
    info->quit = 0;
    return true;
}

bool game_start(){
    DEBUG("game start");
    //TODO : move to a scene manager constract_scene function
    ///
    Mesh plane_m;
    assets_manager_load_asset(&plane_m, file_join_path(ENGINE_RESOURCES_PATH, "objs/plane.obj"),ASSET_TYPE_MODEL);
    MeshRenderer plane_mesh_r = mesh_renderer_create(&plane_m);
    Transform plane_t = {
        .position = {0, -.5f, 0},
        .rotation = {.0f, .0f, .0f},
        .scale = {10.f, 1.f, 10.f}
    };

    EntityID plane_e = ecs_manager_create_entity(0);
    ecs_manager_add_component(0,plane_e,TYPE_NAME(Transform), &plane_t);
    ecs_manager_add_component(0,plane_e,TYPE_NAME(MeshRenderer), &plane_mesh_r);

    Mesh mesh;
    assets_manager_load_asset(&mesh, file_join_path(ENGINE_RESOURCES_PATH, "objs/cube.obj"),ASSET_TYPE_MODEL);

    MeshRenderer mesh_r = mesh_renderer_create(&mesh);
    Transform t = {
        .position = { 2.5f , 0, -3.f},
        .rotation = {0.0f, 0.0f, 0.0f},
        .scale = {1.f, 1.f, 1.f}
    };

    EntityID e = ecs_manager_create_entity(0);
    ecs_manager_add_component(0,e,TYPE_NAME(Transform), &t);
    ecs_manager_add_component(0,e,TYPE_NAME(MeshRenderer), &mesh_r);

    MeshRenderer mesh_r0 = mesh_renderer_create(&mesh);
    Transform t0 = {
        .position = { -2.5f , .5f, -3.f},
        .rotation = {.0f, .0f, .0f},
        .scale = {1.f, 2.f, 3.f}
    };

    EntityID e0 = ecs_manager_create_entity(0);
    ecs_manager_add_component(0,e0,TYPE_NAME(Transform), &t0);
    ecs_manager_add_component(0,e0,TYPE_NAME(MeshRenderer), &mesh_r0);

    // light source with mesh
    LightSource lightSrc = light_source_create_mesh_light(&mesh, 10.f, vec4(.0f,.0f,1.f,1.f));
    Transform t1 = {
        .position = { 0.f , 3.f, -3.f},
        .rotation = {.0f, .0f, .0f},
        .scale = {.5f, .5f, .5f}
    };

    EntityID e1 = ecs_manager_create_entity(0);
    ecs_manager_add_component(0,e1,TYPE_NAME(Transform), &t1);
    ecs_manager_add_component(0,e1,TYPE_NAME(LightSource), &lightSrc);


    // point light
    LightSource pointLight = light_source_create_point_light(4.f, vec4(1.f,.0f,.0f,1.5f));
    
    Transform pointLightT = {
        .position = { 3.f , 3.f, -3.f},
        .rotation = {.0f, .0f, .0f},
        .scale = {.3f, .3f, 1.f}
    };

    EntityID pointLightE = ecs_manager_create_entity(0);
    ecs_manager_add_component(0,pointLightE,TYPE_NAME(Transform), &pointLightT);
    ecs_manager_add_component(0,pointLightE,TYPE_NAME(LightSource), &pointLight);

    // point light
    LightSource pointLight1 = light_source_create_point_light(1.f, vec4(0.f,1.f,.0f,1.f));
    
    Transform pointLightT1 = {
        .position = { 2.f , 3.f, -2.f},
        .rotation = {.0f, .0f, .0f},
        .scale = {.3f, .3f, 1.f}
    };

    EntityID pointLightE1 = ecs_manager_create_entity(0);
    ecs_manager_add_component(0,pointLightE1,TYPE_NAME(Transform), &pointLightT1);
    ecs_manager_add_component(0,pointLightE1,TYPE_NAME(LightSource), &pointLight1);

    // directional light
    LightSource directionalLight1 = light_source_create_directional_light(vec4(1.f,1.f,1.f,1.f), true);
    
    Transform directionalLightT1 = {
        .position = { 2.f , 3.f, -2.f},
        .rotation = {2.0f, -1.f, 3.f},
        .scale = {1.f, 1.f, 1.f}
    };

    EntityID directionalLightE = ecs_manager_create_entity(0);
    ecs_manager_add_component(0,directionalLightE,TYPE_NAME(Transform), &directionalLightT1);
    ecs_manager_add_component(0,directionalLightE,TYPE_NAME(LightSource), &directionalLight1);
    
    ///

    return true;
}