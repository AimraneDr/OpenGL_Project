#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H
#include "component_types.h"

DLL_EXPORT Vec3 transform_get_up(const Transform transform);
DLL_EXPORT Vec3 transform_get_forward(const Transform transform);
DLL_EXPORT Vec3 transform_get_right(const Transform transform);

#endif /* TRANSFORM_COMPONENT_H */