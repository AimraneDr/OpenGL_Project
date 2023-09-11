/**
 * @file light_source.c
 * @author your name (aimranedr@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "components/light_source.h"

#include "components/mesh_renderer.h"
#include "render/vertex_buffer.h"
#include "render/vertex_array.h"
#include "core/logger.h"
#include <stdlib.h>


LightSource light_source_create_mesh_light(Mesh* mesh, f32 radius, Vec4 color){
    LightSource out = {
        .type = LIGHT_SOURCE_TYPE_MESH,
        .radius = radius,
        .color = color
    };
    out.__mr__ = mesh_renderer_create(mesh);
    return out;
}

LightSource light_source_create_point_light(f32 radius, Vec4 color){
    LightSource out = {
        .type = LIGHT_SOURCE_TYPE_POINT,
        .radius = radius,
        .color = color
    };
    Vertex verts[4] = {
        {{-1.0f, -1.0f, 0.f}, {}, {}, {}},
        {{-1.0f,  1.0f, 0.f}, {}, {}, {}},
        {{ 1.0f, -1.0f, 0.f}, {}, {}, {}},
        {{ 1.0f,  1.0f, 0.f}, {}, {}, {}} 
    };
    u32 inds[6] = {
        0,2,1,
        2,3,1
    };
    Mesh mesh = {};
    mesh.vertices = verts;
    mesh.indices = inds;
    mesh.numVertices = 4;
    mesh.numIndices = 6;

    out.__mr__ = mesh_renderer_create(&mesh);

    //free
    return out;
}

bool light_source_render_point_light(LightSource* light_source){

    return true;
}

// bool light_source_create_area_light(LightSource* out, f32 radius, Vec4 color);
LightSource light_source_create_directional_light(Vec4 color, bool cast_shadows){
    LightSource out = {
        .type = LIGHT_SOURCE_TYPE_DIRECTIONAL,
        .color = color,
        .castShadows = cast_shadows,
        .radius = -1,
        .halfAngle = 0
    };
    return out;
}

bool light_source_destroy(LightSource* light){
    if(light->type != LIGHT_SOURCE_TYPE_DIRECTIONAL){
        mesh_renderer_destroy(&light->__mr__);
    }
    light->radius = 0;
    light->color = (Vec4){};
    light->halfAngle = 0;
    light->castShadows = 0;
    return true;
}