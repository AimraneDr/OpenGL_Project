/**
 * @file camera.h
 * @author your name (you@domain.com)
 * @brief 
 * @todo desguss moving the camera system into a component 
 * @version 0.1
 * @date 2023-07-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef CAMERA_H
#define CAMERA_H

#include "defines.h"
#include "engine_types.h"
#include "math/math_defines.h"
#include "shader.h"


Camera camera_obj_init(Camera * out_camera, Vec3 pos, Vec2 rectSize);

Mat4 camera_view_projection_mat(Camera * camera);

bool camera_controller_update(Camera* cam, f64 deltatime);

#endif /* CAMERA_H */