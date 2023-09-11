#include "math/details/vec4.h"

#include "math/details/utils.h"

Vec4 vec4(f32 x, f32 y, f32 z, f32 w){
    Vec4 v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
    return v;
}

Vec4 vec4_add(Vec4 a, Vec4 b){
    Vec4 r;
    r.x = a.x+b.x;
    r.y = a.y+b.y;
    r.y = a.z+b.z;
    r.y = a.w+b.w;
    return r;
}
Vec4 vec4_subtract(Vec4 a, Vec4 b){
    Vec4 r;
    r.x = a.x-b.x;
    r.y = a.y-b.y;
    r.y = a.z-b.z;
    r.y = a.w-b.w;
    return r;
}
f32 vec4_dot(Vec4 a, Vec4 b){
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
Vec4 vec4_multiply(Vec4 a, f32 b){
    Vec4 v;
    v.x = a.x*b;
    v.y = a.y*b;
    v.z = a.z*b;
    v.w = a.w*b;
    return v; 
}

bool vec4_compare(Vec4 a, Vec4 b){
    return f_equal(a.x,b.x) && f_equal(a.y,b.y) && f_equal(a.z,b.z) && f_equal(a.w,b.w);
}

Vec4 vec4_normalize(Vec4 v){
    f32 length = vec4_magnitude(v);
    if (length > 0.0f) {
        v.x /= length;
        v.y /= length;
        v.z /= length;
        v.w /= length;
    }
    return v;
}
f32 vec4_magnitude(Vec4 v){
    return u_sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}