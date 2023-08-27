#include "math/math.h"
#include <math.h>

//TODO : revise and move to place

Vec3 rotate(Vec3 vector, f32 angle, Vec3 axis) {
    float radians = rad(angle);
    float c = cos(radians);
    float s = sin(radians);
    float oneMinusC = 1.0f - c;

    Vec3 result;
    result.x = vector.x * (axis.x * axis.x * oneMinusC + c) +
               vector.y * (axis.x * axis.y * oneMinusC - axis.z * s) +
               vector.z * (axis.x * axis.z * oneMinusC + axis.y * s);

    result.y = vector.x * (axis.x * axis.y * oneMinusC + axis.z * s) +
               vector.y * (axis.y * axis.y * oneMinusC + c) +
               vector.z * (axis.y * axis.z * oneMinusC - axis.x * s);

    result.z = vector.x * (axis.x * axis.z * oneMinusC - axis.y * s) +
               vector.y * (axis.y * axis.z * oneMinusC + axis.x * s) +
               vector.z * (axis.z * axis.z * oneMinusC + c);

    return result;
}
Vec3 rotate_by_mat(Vec3 v, f32 angle, Vec3 a) {
    float radians = rad(angle);
    float cosAngle = cosf(radians);
    float sinAngle = sinf(radians);
    float oneMinusCos = 1.0f - cosAngle;

    // Create the rotation matrix
    float matrix[3][3] = {
        {cosAngle + a.x * a.x * oneMinusCos, a.x * a.y * oneMinusCos - a.z * sinAngle, a.x * a.z * oneMinusCos + a.y * sinAngle},
        {a.y * a.x * oneMinusCos + a.z * sinAngle, cosAngle + a.y * a.y * oneMinusCos, a.y * a.z * oneMinusCos - a.x * sinAngle},
        {a.z * a.x * oneMinusCos - a.y * sinAngle, a.z * a.y * oneMinusCos + a.x * sinAngle, cosAngle + a.z * a.z * oneMinusCos}
    };

    // Perform matrix-vector multiplication
    Vec3 result;
    result.x = matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z;
    result.y = matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z;
    result.z = matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z;

    return result;
}


Vec3 vec3_rotateX(Vec3 v, f32 angle) {
    f32 cosA = cos(angle);
    f32 sinA = sin(angle);

    Vec3 rotated;
    rotated.x = v.x;
    rotated.y = v.y * cosA - v.z * sinA;
    rotated.z = v.y * sinA + v.z * cosA;

    return rotated;
}

Vec3 vec3_rotateY(Vec3 v, f32 angle) {
    f32 cosA = cos(angle);
    f32 sinA = sin(angle);

    Vec3 rotated;
    rotated.x = v.x * cosA + v.z * sinA;
    rotated.y = v.y;
    rotated.z = v.z * cosA - v.x * sinA;

    return rotated;
}

Vec3 vec3_rotateZ(Vec3 v, f32 angle) {
    f32 cosA = cos(angle);
    f32 sinA = sin(angle);

    Vec3 rotated;
    rotated.x = v.x * cosA - v.y * sinA;
    rotated.y = v.x * sinA + v.y * cosA;
    rotated.z = v.z;

    return rotated;
}


/* TEMP */
Mat4 mat4_euler_x(f32 angle_radians) {
    Mat4 out_matrix = mat4(1.f);
    f32 c = cos_t(angle_radians);
    f32 s = sin_t(angle_radians);

    out_matrix.elements[5] = c;
    out_matrix.elements[6] = s;
    out_matrix.elements[9] = -s;
    out_matrix.elements[10] = c;
    return out_matrix;
}
Mat4 mat4_euler_y(f32 angle_radians) {
    Mat4 out_matrix = mat4(1.f);
    f32 c = cos_t(angle_radians);
    f32 s = sin_t(angle_radians);

    out_matrix.elements[0] = c;
    out_matrix.elements[2] = -s;
    out_matrix.elements[8] = s;
    out_matrix.elements[10] = c;
    return out_matrix;
}

Mat4 mat4_euler_z(f32 angle_radians) {
    Mat4 out_matrix = mat4(1.f);

    f32 c = cos_t(angle_radians);
    f32 s = sin_t(angle_radians);

    out_matrix.elements[0] = c;
    out_matrix.elements[1] = s;
    out_matrix.elements[4] = -s;
    out_matrix.elements[5] = c;
    return out_matrix;
}

Mat4 mat4_rotation_xyz(Vec3 r){
    Mat4 rx = mat4_euler_x(r.x);
    Mat4 ry = mat4_euler_y(r.y);
    Mat4 rz = mat4_euler_z(r.z);
    Mat4 out_matrix = mat4_multiply(rx, ry);
    out_matrix = mat4_multiply(out_matrix, rz);
    return out_matrix;
}


Mat4 mat4_rotateY(Mat4* m, f32 angle) {
    f32 cosA = cos(angle);
    f32 sinA = sin(angle);

    f32 m0 = cosA * m->elements[0] - sinA * m->elements[2];
    f32 m2 = sinA * m->elements[0] + cosA * m->elements[2];
    m->elements[0] = m0;
    m->elements[2] = m2;

    f32 m4 = cosA * m->elements[4] - sinA * m->elements[6];
    f32 m6 = sinA * m->elements[4] + cosA * m->elements[6];
    m->elements[4] = m4;
    m->elements[6] = m6;

    f32 m8 = cosA * m->elements[8] - sinA * m->elements[10];
    f32 m10 = sinA * m->elements[8] + cosA * m->elements[10];
    m->elements[8] = m8;
    m->elements[10] = m10;

    f32 m12 = cosA * m->elements[12] - sinA * m->elements[14];
    f32 m14 = sinA * m->elements[12] + cosA * m->elements[14];
    m->elements[12] = m12;
    m->elements[14] = m14;

    return *m;
}

Mat4 mat4_rotateX(Mat4* m, f32 angle) {
    f32 cosA = cos(angle);
    f32 sinA = sin(angle);

    f32 m1 = cosA * m->elements[1] - sinA * m->elements[2];
    f32 m2 = sinA * m->elements[1] + cosA * m->elements[2];
    m->elements[1] = m1;
    m->elements[2] = m2;

    f32 m5 = cosA * m->elements[5] - sinA * m->elements[6];
    f32 m6 = sinA * m->elements[5] + cosA * m->elements[6];
    m->elements[5] = m5;
    m->elements[6] = m6;

    f32 m9 = cosA * m->elements[9] - sinA * m->elements[10];
    f32 m10 = sinA * m->elements[9] + cosA * m->elements[10];
    m->elements[9] = m9;
    m->elements[10] = m10;

    f32 m13 = cosA * m->elements[13] - sinA * m->elements[14];
    f32 m14 = sinA * m->elements[13] + cosA * m->elements[14];
    m->elements[13] = m13;
    m->elements[14] = m14;

    return *m;
}

Mat4 mat4_rotateZ(Mat4* m, f32 angle) {
    f32 cosA = cos(angle);
    f32 sinA = sin(angle);

    f32 m0 = cosA * m->elements[0] - sinA * m->elements[1];
    f32 m1 = sinA * m->elements[0] + cosA * m->elements[1];
    m->elements[0] = m0;
    m->elements[1] = m1;

    f32 m4 = cosA * m->elements[4] - sinA * m->elements[5];
    f32 m5 = sinA * m->elements[4] + cosA * m->elements[5];
    m->elements[4] = m4;
    m->elements[5] = m5;

    f32 m8 = cosA * m->elements[8] - sinA * m->elements[9];
    f32 m9 = sinA * m->elements[8] + cosA * m->elements[9];
    m->elements[8] = m8;
    m->elements[9] = m9;

    f32 m12 = cosA * m->elements[12] - sinA * m->elements[13];
    f32 m13 = sinA * m->elements[12] + cosA * m->elements[13];
    m->elements[12] = m12;
    m->elements[13] = m13;

    return *m;
}

Mat4 mat4_rotateXY(Mat4* m, Vec2 rot) {
    mat4_rotateX(m, rot.x);
    mat4_rotateY(m, rot.y);
    return *m;
}

Mat4 mat4_rotateXZ(Mat4* m, Vec2 rot) {
    mat4_rotateX(m, rot.x);
    mat4_rotateZ(m, rot.y);
    return *m;
}

Mat4 mat4_rotateYX(Mat4* m, Vec2 rot) {
    mat4_rotateY(m, rot.y);
    mat4_rotateX(m, rot.x);
    return *m;
}

Mat4 mat4_rotateYZ(Mat4* m, Vec2 rot) {
    mat4_rotateY(m, rot.y);
    mat4_rotateZ(m, rot.x);
    return *m;
}

Mat4 mat4_rotateZX(Mat4* m, Vec2 rot) {
    mat4_rotateZ(m, rot.y);
    mat4_rotateX(m, rot.x);
    return *m;
}

Mat4 mat4_rotateZY(Mat4* m, Vec2 rot) {
    mat4_rotateZ(m, rot.x);
    mat4_rotateY(m, rot.y);
    return *m;
}

Mat4 mat4_rotateXYZ(Mat4* m, Vec3 rot) {
    mat4_rotateX(m, rot.x);
    mat4_rotateY(m, rot.y);
    mat4_rotateZ(m, rot.z);
    return *m;
}

Mat4 mat4_rotateXZY(Mat4* m, Vec3 rot) {
    mat4_rotateX(m, rot.x);
    mat4_rotateZ(m, rot.z);
    mat4_rotateY(m, rot.y);
    return *m;
}

Mat4 mat4_rotateYXZ(Mat4* m, Vec3 rot) {
    mat4_rotateY(m, rot.y);
    mat4_rotateX(m, rot.x);
    mat4_rotateZ(m, rot.z);
    return *m;
}

Mat4 mat4_rotateYZX(Mat4* m, Vec3 rot) {
    mat4_rotateY(m, rot.y);
    mat4_rotateZ(m, rot.z);
    mat4_rotateX(m, rot.x);
    return *m;
}

Mat4 mat4_rotateZXY(Mat4* m, Vec3 rot) {
    mat4_rotateZ(m, rot.z);
    mat4_rotateX(m, rot.x);
    mat4_rotateY(m, rot.y);
    return *m;
}

Mat4 mat4_rotateZYX(Mat4* m, Vec3 rot) {
    mat4_rotateZ(m, rot.z);
    mat4_rotateY(m, rot.y);
    mat4_rotateX(m, rot.x);
    return *m;
}

Mat4 mat4_rotation(Mat4 m, Vec3 rot){
    return mat4_rotateXYZ(&m, rot);
}

Mat4 mat4_rotate(Mat4* m, Vec3 rot){
    return mat4_rotateXYZ(m, rot);
}

Mat4 mat4_rotate_o(Mat4* m, Vec3 rot, RotationType type){
    switch (type)
    {
        case ROT_X:
            return mat4_rotateX(m, rot.x);
        case ROT_Y:
            return mat4_rotateY(m, rot.y);
        case ROT_Z:
            return mat4_rotateZ(m, rot.z);
        case ROT_XY:
            return mat4_rotateXY(m, vec2(rot.x,rot.y));
        case ROT_XZ:
            return mat4_rotateXZ(m, vec2(rot.x,rot.z));
        case ROT_YX:
            return mat4_rotateYX(m, vec2(rot.x,rot.y));
        case ROT_YZ:
            return mat4_rotateYZ(m, vec2(rot.z,rot.y));
        case ROT_ZX:
            return mat4_rotateZX(m, vec2(rot.x,rot.z));
        case ROT_ZY:
            return mat4_rotateZY(m, vec2(rot.z,rot.y));
        case ROT_XZY:
            return mat4_rotateXZY(m, rot);
        case ROT_YXZ:
            return mat4_rotateYXZ(m, rot);
        case ROT_YZX:
            return mat4_rotateYZX(m, rot);
        case ROT_ZXY:
            return mat4_rotateZXY(m, rot);
        case ROT_ZYX:
            return mat4_rotateZYX(m, rot);
        case ROT_DEFAULT:
        case ROT_XYZ:
        default:
            return mat4_rotateXYZ(m, rot);
    }
}