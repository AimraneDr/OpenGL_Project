#include "math/details/vec2.h"

#include "math/details/utils.h"

Vec2 vec2(f32 x, f32 y){
    Vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

Vec2 vec2_add(Vec2 a, Vec2 b){
    Vec2 r;
    r.x = a.x+b.x;
    r.y = a.y+b.y;
    return r;
}

Vec2 vec2_subtract(Vec2 a, Vec2 b){
    Vec2 r;
    r.x = a.x-b.x;
    r.y = a.y-b.y;
    return r;
}

f32 vec2_dot(Vec2 a, Vec2 b){
    return a.x * b.x + a.y * b.y;
}

Vec2 vec2_scalar_multiply(Vec2 a, f32 b){
    Vec2 v;
    v.x = a.x*b;
    v.y = a.y*b;
    return v; 
}

bool vec2_compare(Vec2 a, Vec2 b){
    return f_equal(a.x,b.x) && f_equal(a.y,b.y);
}

Vec2 vec2_normalize(Vec2 v){
    f32 magnitude = (v.x * v.x) + (v.y * v.y);
    Vec2 result;
    result.x = power(v.x,2) / magnitude;
    result.y = power(v.y,2) / magnitude;
    return result;
}