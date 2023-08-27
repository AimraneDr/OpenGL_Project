#ifndef QUATERNION_DETAILS_H
#define QUATERNION_DETAILS_H

#include "defines.h"
#include "math/math_defines.h"

DLL_EXPORT Quaternion quaternion_multiply(Quaternion q1, Quaternion q2);
DLL_EXPORT Quaternion quaternion_conjugate(Quaternion q);

#endif /* QUATERNION_DETAILS_H */