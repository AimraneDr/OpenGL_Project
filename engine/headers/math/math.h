#ifndef MATH_H
#define MATH_H

#include "defines.h"
#include "math_defines.h"

#include "details/utils.h"
#include "details/trigonometry.h"
#include "details/vec2.h"
#include "details/vec3.h"
#include "details/vec4.h"
#include "details/quaternion.h"
#include "details/mat3x3.h"
#include "details/mat4x4.h"



// Vector operations


DLL_EXPORT Vec3 rotate(Vec3 vector, f32 angle, Vec3 axis);
DLL_EXPORT Vec3 rotate_by_mat(Vec3 v, f32 angle, Vec3 a);

DLL_EXPORT Vec3 vec3_rotateX(Vec3 v, f32 angle);
DLL_EXPORT Vec3 vec3_rotateY(Vec3 v, f32 angle);
DLL_EXPORT Vec3 vec3_rotateZ(Vec3 v, f32 angle);

#endif /* MATH_H */