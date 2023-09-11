#ifndef MATRIX3X3_DETAILS_H
#define MATRIX3X3_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT Mat3 mat3(f32 x);
DLL_EXPORT f32* mat3_get(Mat3* m, u8 col, u8 row);
DLL_EXPORT Mat3 mat3_add(Mat3 a, Mat3 b);
DLL_EXPORT Mat3 mat3_subtract(Mat3 a, Mat3 b);
DLL_EXPORT Mat3 mat3_multiply(Mat3 a, Mat3 b);
DLL_EXPORT Mat3 mat3_scalar_multiply(Mat3 a, f32 b);
DLL_EXPORT bool mat3_compare(Mat3 a, Mat3 b);

#endif /* MATRIX3X3_DETAILS_H */