#include "components/transform.h"

#include "core/logger.h"
#include "core/CDS/strings.h"
#include <math.h>
#include "math/math.h"


Vec3 transform_get_forward(const Transform transform) {
    Mat4 m = mat4(1.f);
    Vec3 rad_rot = {
        rad(transform.rotation.x),
        rad(transform.rotation.y),
        rad(transform.rotation.z)
    };
    m = mat4_translate(m,vec3_forward());
    m = mat4_rotate(&m,rad_rot);

    // Calculate the forward vector using the rotation matrix
    Vec3 forward;
    forward.x = -m.elements[8];
    forward.y = -m.elements[9];
    forward.z = -m.elements[10];
    forward = vec3_normalize(forward);
    return forward;
}

Vec3 transform_get_up(const Transform transform) {
    Mat4 m = mat4(1.f);
    Vec3 rad_rot = {
        rad(transform.rotation.x),
        rad(transform.rotation.y),
        rad(transform.rotation.z)
    };
    m = mat4_translate(m,vec3_up());
    m = mat4_rotate(&m,rad_rot);

    // Calculate the up vector using the rotation matrix
    Vec3 up;
    up.x = m.elements[4];
    up.y = m.elements[5];
    up.z = m.elements[6];
    up = vec3_normalize(up);
    return up;
}

Vec3 transform_get_right(const Transform transform) {
    Mat4 m = mat4(1.f);
    Vec3 rad_rot = {
        rad(transform.rotation.x),
        rad(transform.rotation.y),
        rad(transform.rotation.z)
    };
    m = mat4_translate(m,vec3_right());
    m = mat4_rotate(&m,rad_rot);

    // Calculate the right vector using the rotation matrix
    Vec3 right;
    right.x = m.elements[0];
    right.y = m.elements[1];
    right.z = m.elements[2];
    right = vec3_normalize(right);
    return right;
}