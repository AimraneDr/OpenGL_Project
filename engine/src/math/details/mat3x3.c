#include "math/details/mat3x3.h"

#include "math/details/utils.h"

Mat3 mat3(f32 x){
    Mat3 m;
    m.elements[0] = x;
    m.elements[1] = .0f;
    m.elements[2] = .0f;

    m.elements[3] = .0f;
    m.elements[4] = x;
    m.elements[5] = .0f;

    m.elements[6] = .0f;
    m.elements[7] = .0f;
    m.elements[8] = x;
    return m;
}
Mat3 mat3_add(Mat3 a, Mat3 b){
    Mat3 r;
    for (int i = 0; i < 9; i++) {
        r.elements[i] = a.elements[i] + b.elements[i];
    }
    return r;
}
Mat3 mat3_subtract(Mat3 a, Mat3 b){
    Mat3 r;
    for (int i = 0; i < 9; i++) {
        r.elements[i] = a.elements[i] - b.elements[i];
    }
    return r;
}
Mat3 mat3_multiply(Mat3 a, Mat3 b){
    Mat3 r;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            f32 sum = 0;
            for (int k = 0; k < 3; k++) {
                sum += a.elements[i * 3 + k] * b.elements[k * 3 + j];
            }
            r.elements[i * 3 + j] = sum;
        }
    }
    return r;
}
Mat3 mat3_scalar_multiply(Mat3 a, f32 b){
    Mat3 r;
    for (int i = 0; i < 9; i++) {
        r.elements[i] = a.elements[i] * b;
    }
    return r;
}

bool mat3_compare(Mat3 a,Mat3 b){
    for (int i = 0; i < 9; i++) if(!f_equal(a.elements[i], b.elements[i])) return false;
    return true;
}
f32* mat3_get(Mat3* m, u8 col, u8 row) {
    return &(m->elements[col * 4 + row]);
}