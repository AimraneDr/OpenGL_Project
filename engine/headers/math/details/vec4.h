#ifndef VEC4_DETAILS_H
#define VEC4_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT Vec4 vec4(f32 x, f32 y, f32 z, f32 w);
DLL_EXPORT Vec4 vec4_add(Vec4 a, Vec4 b);
DLL_EXPORT Vec4 vec4_subtract(Vec4 a, Vec4 b);
DLL_EXPORT f32 vec4_dot(Vec4 a, Vec4 b);
DLL_EXPORT Vec4 vec4_multiply(Vec4 a, f32 b);
DLL_EXPORT bool vec4_compare(Vec4 a, Vec4 b);
DLL_EXPORT Vec4 vec4_normalize(Vec4 v);
DLL_EXPORT f32 vec4_magnitude(Vec4 v);


#endif /* VEC4_DETAILS_H */