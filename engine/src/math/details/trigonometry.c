#include "math/details/trigonometry.h"

#include <math.h>

f32 deg(f32 rad){
    double angle = fmod(rad, TWO_PI);
    if (angle < 0)  angle += TWO_PI;    
    return angle * (180.0 / PI);
}
f32 rad(f32 deg){
    f32 angle = fmod(deg, 360.0f);
    if (angle < 0)  angle += 360.0;
    return angle * (PI / 180.0);
}

f32 cos_t(f32 angle) {
    return cos(angle);
}

f32 sin_t(f32 angle) {
    return sin(angle);
}

f32 tan_t(f32 angle) {
    return tan(angle);
}

f32 acos_t(f32 dot) {
    return acosf(dot);
}

f32 asin_t(f32 dot) {
    return asinf(dot);
}

f32 atan_t(f32 dot) {
    return atanf(dot);
}

f32 atan2_t(f32 x,f32 y) {
    return atan2f(x,y);
}