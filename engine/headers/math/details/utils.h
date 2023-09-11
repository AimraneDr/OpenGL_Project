#ifndef MATH_UTILS_DETAILS_H
#define MATH_UTILS_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT u32 factorial(u32 n);
DLL_EXPORT f64 power(f64 x, u32 n);

/// @brief compare floats a & b
/// @param a 
/// @param b 
/// @return returns 0 if equal, 1 if a > b , and -1 if a < b
DLL_EXPORT s8 f_compare(f32 a, f32 b);
DLL_EXPORT bool f_equal(f32 a, f32 b);
DLL_EXPORT f32 u_sqrt(f32 a);

DLL_EXPORT s32 rand32(s32 min, s32 max);

#define ABS(a) a < 0 ? -a : a 

#endif /* MATH_UTILS_DETAILS_H */