#ifndef VEC2_DETAILS_H
#define VEC2_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT Vec2 vec2(f32 x, f32 y);
DLL_EXPORT Vec2 vec2_add(Vec2 a, Vec2 b);
DLL_EXPORT Vec2 vec2_subtract(Vec2 a, Vec2 b);
DLL_EXPORT f32 vec2_dot(Vec2 a, Vec2 b);
DLL_EXPORT Vec2 vec2_scalar_multiply(Vec2 a, f32 b);
DLL_EXPORT bool vec2_compare(Vec2 a, Vec2 b);

#endif /* VEC2_DETAILS_H */