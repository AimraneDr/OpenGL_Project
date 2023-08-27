#ifndef TRIGONOMETRY_DETAILS_H
#define TRIGONOMETRY_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT f32 deg(f32 rad);
DLL_EXPORT f32 rad(f32 deg);

DLL_EXPORT f32 cos_t(f32 angle);
DLL_EXPORT f32 sin_t(f32 angle);
DLL_EXPORT f32 tan_t(f32 angle);

DLL_EXPORT f32 acos_t(f32 dot);
DLL_EXPORT f32 asin_t(f32 dot);
DLL_EXPORT f32 atan_t(f32 dot);
DLL_EXPORT f32 atan2_t(f32 x,f32 y);

#endif /* TRIGONOMETRY_DETAILS_H */