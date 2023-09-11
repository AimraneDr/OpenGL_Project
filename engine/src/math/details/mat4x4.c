#include "math/details/mat4x4.h"

#include "math/details/utils.h"
#include "math/details/trigonometry.h"
#include "math/details/vec3.h"

Mat4 mat4(f32 x){
    Mat4 m;
    for (int i = 0; i < 16; i++) {
        m.elements[i] = 0.0f;
    }
    m.elements[0] = x;
    m.elements[5] = x;
    m.elements[10] = x;
    m.elements[15] = x;
    return m;
}

Mat4 mat4_add(Mat4 a, Mat4 b){
    Mat4 r;
    for (int i = 0; i < 16; i++) {
        r.elements[i] = a.elements[i] + b.elements[i];
    }
    return r;
}
Mat4 mat4_subtract(Mat4 a, Mat4 b){
    Mat4 r;
    for (int i = 0; i < 16; i++) {
        r.elements[i] = a.elements[i] - b.elements[i];
    }
    return r;
}
Mat4 mat4_multiply(Mat4 a, Mat4 b){
    Mat4 result;

    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += a.elements[row + k * 4] * b.elements[k + col * 4];
            }
            result.elements[row + col * 4] = sum;
        }
    }

    return result;
}
Mat4 mat4_scalar_multiply(Mat4 a, f32 b){
    Mat4 r;
    for (int i = 0; i < 16; i++) {
        r.elements[i] = a.elements[i] * b;
    }
    return r;
}

bool mat4_compare(Mat4 a,Mat4 b){
    for (int i = 0; i < 16; i++) {
            if(!f_equal(a.elements[i], b.elements[i])) return false;
    }
    return true;
}

f32* mat4_get(Mat4* m, u8 col, u8 row) {
    return &(m->elements[col * 4 + row]);
}

Mat4 mat4_scale(Mat4 m, Vec3 s) {
    m.elements[0] *= s.x;
    m.elements[5] *= s.y;
    m.elements[10] *= s.z;
    return m;
}

Mat4 mat4_translate(Mat4 m, Vec3 t){
    m.elements[12] = t.x;
    m.elements[13] = t.y;
    m.elements[14] = t.z;
    return m;
}
Mat4 mat4_translation(Vec3 position) {
    Mat4 out_matrix = mat4(1.f);
    out_matrix.elements[12] = position.x;
    out_matrix.elements[13] = position.y;
    out_matrix.elements[14] = position.z;
    return out_matrix;
}

Mat4 mat4_inverse(Mat4 matrix){
    const f32* m = matrix.elements;

    f32 t0 = m[10] * m[15];
    f32 t1 = m[14] * m[11];
    f32 t2 = m[6] * m[15];
    f32 t3 = m[14] * m[7];
    f32 t4 = m[6] * m[11];
    f32 t5 = m[10] * m[7];
    f32 t6 = m[2] * m[15];
    f32 t7 = m[14] * m[3];
    f32 t8 = m[2] * m[11];
    f32 t9 = m[10] * m[3];
    f32 t10 = m[2] * m[7];
    f32 t11 = m[6] * m[3];
    f32 t12 = m[8] * m[13];
    f32 t13 = m[12] * m[9];
    f32 t14 = m[4] * m[13];
    f32 t15 = m[12] * m[5];
    f32 t16 = m[4] * m[9];
    f32 t17 = m[8] * m[5];
    f32 t18 = m[0] * m[13];
    f32 t19 = m[12] * m[1];
    f32 t20 = m[0] * m[9];
    f32 t21 = m[8] * m[1];
    f32 t22 = m[0] * m[5];
    f32 t23 = m[4] * m[1];

    Mat4 out_matrix;
    f32* o = out_matrix.elements;

    o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
    o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
    o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
    o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

    f32 d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

    o[0] = d * o[0];
    o[1] = d * o[1];
    o[2] = d * o[2];
    o[3] = d * o[3];
    o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
    o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
    o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
    o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
    o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
    o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
    o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
    o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
    o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
    o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
    o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
    o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

    return out_matrix;
}


Mat4 mat4_perspective(f32 fov, f32 aRatio, f32 near, f32 far){
    f32 tanHalfFOV = tan_t(fov * .5f);
    f32 range_inverse = 1.f / (near - far);
    
    Mat4 m = mat4(0.f);
    m.elements[0] = 1.0f / (aRatio * tanHalfFOV);
    m.elements[5] = 1.0f / tanHalfFOV;
    m.elements[10] = (near + far) * range_inverse;
    m.elements[11] = -1.0f;
    m.elements[14] = 2.0f * near * far * range_inverse;
    return m;
}
#include "core/logger.h"
#include "core/CDS/strings.h"
Mat4 mat4_look_at(Vec3 position, Vec3 target, Vec3 up) {
    Mat4 out_matrix;
    Vec3 z_axis = vec3_subtract(target, position);
    z_axis = vec3_normalize(z_axis);

    Vec3 x_axis = vec3_normalize(vec3_cross(z_axis, up));
    Vec3 y_axis = vec3_cross(x_axis, z_axis);

    out_matrix.elements[0] = x_axis.x;
    out_matrix.elements[1] = y_axis.x;
    out_matrix.elements[2] = -z_axis.x;
    out_matrix.elements[3] = 0;
    out_matrix.elements[4] = x_axis.y;
    out_matrix.elements[5] = y_axis.y;
    out_matrix.elements[6] = -z_axis.y;
    out_matrix.elements[7] = 0;
    out_matrix.elements[8] = x_axis.z;
    out_matrix.elements[9] = y_axis.z;
    out_matrix.elements[10] = -z_axis.z;
    out_matrix.elements[11] = 0;
    out_matrix.elements[12] = -vec3_dot(x_axis, position);
    out_matrix.elements[13] = -vec3_dot(y_axis, position);
    out_matrix.elements[14] = vec3_dot(z_axis, position);
    out_matrix.elements[15] = 1.0f;

    return out_matrix;
}


Vec3 extractForwardVector(const Mat4 matrix) {
    Vec3 forwardVector;
    forwardVector.x = matrix.elements[8];
    forwardVector.y = matrix.elements[9];
    forwardVector.z = matrix.elements[10];
    return forwardVector;
}

Vec3 extractUpVector(const Mat4 matrix) {
    Vec3 upVector;
    upVector.x = matrix.elements[4];
    upVector.y = matrix.elements[5];
    upVector.z = matrix.elements[6];
    return upVector;
}

Vec3 extractRightVector(const Mat4 matrix) {
    Vec3 rightVector;
    rightVector.x = matrix.elements[0];
    rightVector.y = matrix.elements[1];
    rightVector.z = matrix.elements[2];
    return rightVector;
}