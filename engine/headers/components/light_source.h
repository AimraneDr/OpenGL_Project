/**
 * @file light_source.h
 * @author your name (aimranedr@gmail.com)
 * @brief functionality for LightSource component
 * @version 0.1
 * @date 2023-07-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef LIGHT_SOURCE_COMPONENT_H
#define LIGHT_SOURCE_COMPONENT_H

#include "component_types.h"

/**
 * @brief create a shape based light source
 * 
 * @param mesh mesh data
 * @param radius max distance that the light's ray can reach
 * @param color the color of the light
 * @return LightSource 
 */
DLL_EXPORT LightSource light_source_create_mesh_light(Mesh* mesh, f32 radius, Vec4 color);
/**
 * @brief create a point light based on a position stored in the transform component
 * 
 * @param radius max distance that the light's ray can reach
 * @param color the color of the light
 * @return LightSource 
 */
DLL_EXPORT LightSource light_source_create_point_light(f32 radius, Vec4 color);

/**
 * @brief 
 * 
 * @param color 
 * @param cast_shadows 
 * @return LightSource 
 */
DLL_EXPORT LightSource light_source_create_directional_light(Vec4 color, bool cast_shadows);

bool light_source_render_point_light(LightSource* light_source);
// bool light_source_create_area_light(LightSource* out, f32 radius, Vec4 color);

DLL_EXPORT bool light_source_destroy(LightSource* light);

#endif /* LIGHT_SOURCE_COMPONENT_H */