#include "render/camera.h"

#include <glad/glad.h>
#include "core/CDS/strings.h"
#include "core/events.h"
#include "core/logger.h"
#include "core/inputs.h"
#include "math/math.h"

#include "components/transform.h"

bool camera_on_window_resize(u16 code, void *sender, void *listener_inst, EventContext data){
    Camera* listenr = (Camera*)listener_inst;
    listenr->rectSize.x = data.data.u16[0];
    listenr->rectSize.y = data.data.u16[1];
    listenr->projection_deprecated = true;
    return true;
}

Camera camera_obj_init(Camera * out_camera, Vec3 pos, Vec2 rectSize){
    out_camera->transform.position = pos;
    out_camera->transform.rotation = vec3(.0f,.0f,.0f);
    out_camera->rectSize = rectSize;
    out_camera->clipping = vec2(.1f,1000.f);
    out_camera->fieldOfView = 45.f;
    out_camera->speed = 4.f;
    out_camera->sensitivity = 100.f;
    out_camera->projection_deprecated = true;
    out_camera->view_deprecated = true;
    out_camera->projctionMatrix = mat4(1.f);
    out_camera->viewMatrix = mat4(1.f);
    out_camera->projViewMatrix = mat4(1.f);
    event_add_listner(EVENT_CODE_RESIZE, out_camera, camera_on_window_resize);

#ifdef USE_DEFAULT_CAMERA_CONTROLLER
    //eable default controller
#endif

    return *out_camera;
}


Mat4 camera_view_projection_mat(Camera * camera){
    if(camera->view_deprecated){
        Vec3 target = vec3_add(
            transform_get_forward(camera->transform),
            camera->transform.position
        );
        camera->viewMatrix = mat4_look_at(camera->transform.position,target,transform_get_up(camera->transform));
    }

    if(camera->projection_deprecated){
        camera->projctionMatrix = mat4_perspective(rad(camera->fieldOfView), camera->rectSize.x/camera->rectSize.y, camera->clipping.x, camera->clipping.y);
    }
    if(camera->projection_deprecated || camera->view_deprecated){
        camera->projViewMatrix = mat4_multiply(camera->projctionMatrix, camera->viewMatrix);
        camera->view_deprecated = false;
        camera->projection_deprecated = false;
    }        
    return camera->projViewMatrix;
}

void camera_look_at(Camera* cam, Vec3 target){
    //fix the camera on a target
}

bool camera_controller_update(Camera* cam, f64 deltatime){
    f32 move_dist = 1.f;
    f32 turn_angle = .5f;
    //movement
    if(input_is_key_down(KEY_W)){
        //forward
        Vec3 move_dir = transform_get_forward(cam->transform);
        cam->transform.position.z += move_dir.z * move_dist * cam->speed * deltatime;
        cam->transform.position.x += move_dir.x * move_dist * cam->speed * deltatime;
        cam->view_deprecated = true;
    }else if(input_is_key_down(KEY_S)){
        //backward
        Vec3 move_dir = transform_get_forward(cam->transform);
        cam->transform.position.z -= move_dir.z * move_dist * cam->speed * deltatime;
        cam->transform.position.x -= move_dir.x * move_dist * cam->speed * deltatime;
        cam->view_deprecated = true;
    }
    if(input_is_key_down(KEY_A)){
        //left
        Vec3 move_dir = transform_get_right(cam->transform);
        cam->transform.position.z -= move_dir.z * move_dist * cam->speed * deltatime;
        cam->transform.position.x -= move_dir.x * move_dist * cam->speed * deltatime;
        cam->view_deprecated = true;
    }else if(input_is_key_down(KEY_D)){
        //right
        Vec3 move_dir = transform_get_right(cam->transform);
        cam->transform.position.z += move_dir.z * move_dist * cam->speed * deltatime;
        cam->transform.position.x += move_dir.x * move_dist * cam->speed * deltatime;
        cam->view_deprecated = true;
    }
    if(input_is_key_down(KEY_Q)){
        //up
        cam->transform.position.y += (move_dist * cam->speed * deltatime);
        cam->view_deprecated = true;
    }else if(input_is_key_down(KEY_E)){
        //down
        cam->transform.position.y -= (move_dist * cam->speed * deltatime);
        cam->view_deprecated = true;
    }

    //rotate
    if(input_is_key_down(KEY_UP)){
        //look up
        if(cam->transform.rotation.x < 45.f){
            cam->transform.rotation.x += turn_angle * cam->sensitivity * deltatime;
            cam->view_deprecated = true;
        }
    }else if(input_is_key_down(KEY_DOWN)){
        //look down
        if(cam->transform.rotation.x > -75.f){
            cam->transform.rotation.x -= turn_angle * cam->sensitivity * deltatime;
            cam->view_deprecated = true;
        }
    }
    if(input_is_key_down(KEY_LEFT)){
        //turn left
        cam->transform.rotation.y -= turn_angle * cam->sensitivity * deltatime;
        cam->view_deprecated = true;
    }else if(input_is_key_down(KEY_RIGHT)){
        //turn right
        cam->transform.rotation.y += turn_angle * cam->sensitivity * deltatime;
        cam->view_deprecated = true;
    }
    return true;
}