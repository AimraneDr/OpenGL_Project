#include "math/details/vec3.h"

#include "math/details/utils.h"
#include <math.h>

Vec3 vec3(f32 x, f32 y, f32 z){
    Vec3 v = { x, y, z };
    return v;
}
Vec3 vec3_add(Vec3 a, Vec3 b){
    Vec3 r = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    };
    return r;
}
Vec3 vec3_subtract(Vec3 a, Vec3 b){
    Vec3 r = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    };
    return r;
}
f32 vec3_dot(Vec3 a, Vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
Vec3 vec3_cross(Vec3 a, Vec3 b){
    Vec3 r = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return r;
}
Vec3 vec3_multiply(Vec3 a, f32 b){
    Vec3 v = {
        a.x * b,
        a.y * b,
        a.z * b
    };
    return v; 
}

bool vec3_compare(Vec3 a, Vec3 b){
    return f_equal(a.x,b.x) && f_equal(a.y,b.y) && f_equal(a.z,b.z);
}

f32 vec3_magnitude(const Vec3 v){
    return u_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

f32 vec3_magnitude_sqr(const Vec3 v){
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

Vec3 vec3_normalize(const Vec3 v){
    f32 sqr_magnitude = vec3_magnitude(v);
    Vec3 r = {
        v.x / sqr_magnitude,
        v.y / sqr_magnitude,
        v.z / sqr_magnitude
    };
    return r;
}

Vec3 vec3_up(){
    return vec3(0.f,1.f,0.f);
}
Vec3 vec3_forward(){
    return vec3(0.f,0.f,-1.f);
}
Vec3 vec3_right(){
    return vec3(1.f,0.f,0.f);
}
Vec3 vec3_down(){
    return vec3(0.f,-1.f,0.f);
}
Vec3 vec3_backward(){
    return vec3(0.f,0.f,1.f);
}
Vec3 vec3_left(){
    return vec3(-1.f,0.f,0.f);
}

f32 vec3_calculate_angle(Vec3 v1, Vec3 v2){
    float dotProduct = vec3_dot(v1,v2);
    float magnitude1 = vec3_magnitude(v1);
    float magnitude2 = vec3_magnitude(v2);


    float cosine = dotProduct / (magnitude1 * magnitude2);

    // Ensure the cosine value is within the valid range [-1, 1].
    cosine = fmaxf(-1.0f, fminf(1.0f, cosine));

    // Calculate the angle in radians.
    float angleRad = acosf(cosine);

    return angleRad;
}
//rotation
