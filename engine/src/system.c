#include "system.h"

#include "game_types.h"
#include "platform/platform.h"
#include "core/logger.h"
#include "core/CDS/strings.h"
#include "core/CDS/dynamic_array.h"
#include "core/events.h"
#include "core/inputs.h"
#include "core/clock.h"
#include "core/files.h"

#include "math/math.h"

#include "render/camera.h"
#include "components/mesh_renderer.h"
#include "assets_manager/assets_manager.h"

//TODO : move to rendering layer
#include <GL/gl.h>
#include <GL/glx.h>

#include <stdlib.h>

#include "core/CDS/unoredered_map.h"

#include "ecs/ecs_manager.h"

#include "systems/render/mainMeshRendererSystem.h"
#include "systems//render/light_source_render_system.h"


bool CloseApp(u16 code, void *sender, void *listener_inst, EventContext data){
    SystemState* sysState = (SystemState*)listener_inst;
    sysState->shouldClose = true;
    return true;
}

#include "core/CDS/details/hash.h"

bool systems_init(SystemState* sysState, GameSetupInfo* game_info){
    INFO("Initializing required subsystems...");
    logger_sys_init();
    events_sys_init();
    inputs_sys_init();

    windowConfig wconfig = {
        .name = game_info->window_info.name,
        .width = game_info->window_info.width,
        .height = game_info->window_info.height,
        .x = game_info->window_info.x,
        .y = game_info->window_info.y,
        .resizable = false
    };
    sysState->shouldClose = false;
    sysState->client_setup = game_info->setup;
    sysState->client_start = game_info->start;
    sysState->client_update = game_info->update;
    sysState->client_quit = game_info->quit;
    
    
    platform_start(wconfig, &sysState->platform);
    

    ecs_manager_init(0);

    //register default components
    ecs_manager_register_component(0, TYPE_NAME(Transform), sizeof(Transform));
    ecs_manager_register_component(0, TYPE_NAME(LightSource), sizeof(LightSource));
    ecs_manager_register_component(0, TYPE_NAME(MeshRenderer), sizeof(MeshRenderer));
    

    //register systems

    //register MainMeshRendererSystem
    MainMeshRendererSystem* mesh_renderer = (MainMeshRendererSystem*)malloc(sizeof(MainMeshRendererSystem));
    SystemRegisterInfo info0 = {
        .systemName = TYPE_NAME(MainMeshRendererSystem),
        .self = mesh_renderer,
        .start = main_mesh_renderer_system_start,
        .preUpdate = main_mesh_renderer_system_pre_update,
        .update = main_mesh_renderer_system_update,
        .AfterUpdate = main_mesh_renderer_system_after_update,
        .shutdown = main_mesh_renderer_system_shutdown
    };
    ecs_manager_register_system(0, info0);
    ecs_manager_set_system_signature(
        0, TYPE_NAME(MainMeshRendererSystem),
        ecs_manager_get_component_type_signature(TYPE_NAME(Transform)) |
        ecs_manager_get_component_type_signature(TYPE_NAME(MeshRenderer))
    );

    //LightSourceRenderSystem
    LightSourceRenderSystem* light_source_renderer = (LightSourceRenderSystem*)malloc(sizeof(MainMeshRendererSystem));
    light_source_renderer->rSys = mesh_renderer;
    SystemRegisterInfo info1 = {
        .systemName = TYPE_NAME(LightSourceRenderSystem),
        .self = light_source_renderer,
        .start = light_source_render_system_start,
        .preUpdate = 0,
        .update = light_source_render_system_update,
        .AfterUpdate = 0,
        .shutdown = light_source_render_system_shutdown
    };
    ecs_manager_register_system(0, info1);
    ecs_manager_set_system_signature(
        0, TYPE_NAME(LightSourceRenderSystem),
        ecs_manager_get_component_type_signature(TYPE_NAME(Transform)) |
        ecs_manager_get_component_type_signature(TYPE_NAME(LightSource))
    );


    if(sysState->client_setup != 0) sysState->client_setup();

    //subscribe to events
    event_add_listner(EVENT_CODE_APPLICATION_QUIT, sysState, CloseApp);

    INFO("All required subsystems had been Initialized.");
    return true;
}


bool systems_run(SystemState* sysState){
    Clock clock={};
    clock_start(&clock);

    Camera main_camera;
    camera_obj_init(&main_camera,vec3(0.f,.5f,5.f), vec2((f32)sysState->platform.w_state.width,(f32)sysState->platform.w_state.height));
    sysState->frame.camera = &main_camera;
    //event_add_listner(EVENT_CODE_RESIZE, sysState->frame.camera, on_window_resize_set_camera_rect);

    if(sysState->client_start != 0) sysState->client_start();

    ecs_manager_systems_start(0);


    while (!sysState->shouldClose)
    {
        platform_window_proccess_events(&sysState->platform.w_state);
        
        if(sysState->shouldClose) break;
        clock_update(&clock);
        sysState->frame.deltaTime = clock.elapsed - clock.preElapsed;

        camera_controller_update(&main_camera, sysState->frame.deltaTime);
        if(input_is_key_down(KEY_F) && input_was_key_up(KEY_F)){
            DEBUG("%.2f fps", 1.f/ sysState->frame.deltaTime)
        }

        //render related
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.12f,.18f,.22f,1.f);


        ecs_manager_systems_update(0, &sysState->frame);

        if(sysState->client_update != 0) sysState->client_update(&sysState->frame);

        //TODO : move to rendering layer
        glXSwapBuffers(sysState->platform.w_state.display, sysState->platform.w_state.window);
        input_update(sysState->frame.deltaTime);
    }
    
    //stop and shut down all registered systems
    ecs_manager_systems_shutdown(0);

    if(sysState->client_quit != 0) sysState->client_quit();

    clock_stop(&clock);
    return true;
}


bool systems_sutdown(SystemState* sysState){
    INFO("Shutting down...")
    ecs_manager_shutdown(0);
    //render_pipeline_destroy(&main_pipeline);
    return true;
}