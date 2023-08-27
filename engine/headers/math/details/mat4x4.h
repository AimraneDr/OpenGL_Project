#ifndef MATRIX4X4_DETAILS_H
#define MATRIX4X4_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT Mat4 mat4(f32 x);
DLL_EXPORT f32* mat4_get(Mat4* m, u8 col, u8 row);
DLL_EXPORT Mat4 mat4_add(Mat4 a, Mat4 b);
DLL_EXPORT Mat4 mat4_subtract(Mat4 a, Mat4 b);
DLL_EXPORT Mat4 mat4_multiply(Mat4 a, Mat4 b);
DLL_EXPORT Mat4 mat4_scalar_multiply(Mat4 a, f32 b);
DLL_EXPORT bool mat4_compare(Mat4 a, Mat4 b);
DLL_EXPORT Mat4 mat4_scale(Mat4 m, Vec3 s);
DLL_EXPORT Mat4 mat4_translate(Mat4 m, Vec3 t);
DLL_EXPORT Mat4 mat4_translation(Vec3 position);
DLL_EXPORT Mat4 mat4_inverse(Mat4 mmatrix);

/// @brief initialize a perspective projection matrix
/// @param fov Field Of View angle in radian
/// @param aRatio Aspect ratio (width / height)
/// @param near Near clipping plane
/// @param far Far clipping plane
/// @return perspctive Matrix 4x4
DLL_EXPORT Mat4 mat4_perspective(f32 fov, f32 aRatio, f32 near, f32 far);

DLL_EXPORT Mat4 mat4_look_at(Vec3 position, Vec3 target, Vec3 up);
DLL_EXPORT Mat4 mat4_rotation(Mat4 m, Vec3 rot);
DLL_EXPORT Mat4 mat4_rotate(Mat4* m, Vec3 rot);
DLL_EXPORT Mat4 mat4_rotate_o(Mat4* m, Vec3 rot, RotationType type);

DLL_EXPORT Mat4 mat4_rotation_xyz(Vec3 r);

//temp
DLL_EXPORT Vec3 extractForwardVector(const Mat4 matrix);
DLL_EXPORT Vec3 extractUpVector(const Mat4 matrix);
DLL_EXPORT Vec3 extractRightVector(const Mat4 matrix);


#endif /* MATRIX4X4_DETAILS_H */