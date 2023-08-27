#ifndef VEC3_DETAILS_H
#define VEC3_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT Vec3 vec3(f32 x, f32 y, f32 z);
DLL_EXPORT Vec3 vec3_add(Vec3 a, Vec3 b);
DLL_EXPORT Vec3 vec3_subtract(Vec3 a, Vec3 b);
DLL_EXPORT f32 vec3_dot(Vec3 a, Vec3 b);
DLL_EXPORT Vec3 vec3_cross(Vec3 a, Vec3 b);
DLL_EXPORT Vec3 vec3_multiply(Vec3 a, f32 b);
DLL_EXPORT bool vec3_compare(Vec3 a, Vec3 b);
DLL_EXPORT Vec3 vec3_normalize(const Vec3 v);
DLL_EXPORT f32 vec3_magnitude(const Vec3 v);
DLL_EXPORT f32 vec3_magnitude_sqr(const Vec3 v);
DLL_EXPORT f32 vec3_calculate_angle(Vec3 v1, Vec3 v2);

DLL_EXPORT Vec3 vec3_up();
DLL_EXPORT Vec3 vec3_forward();
DLL_EXPORT Vec3 vec3_right();
DLL_EXPORT Vec3 vec3_down();
DLL_EXPORT Vec3 vec3_backward();
DLL_EXPORT Vec3 vec3_left();

//rotation

#endif /* VEC3_DETAILS_H */