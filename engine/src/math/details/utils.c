#include "math/details/utils.h"

#include <math.h>
#include "core/logger.h"
#include <stdlib.h>
#include <time.h>

u32 factorial(u32 n) {
    u32 result = 1;
    for (u32 i = n; i >= 2; i--) {
        result *= i;
    }
    return result;
}

f64 power(f64 x, u32 n) {
    for (u32 i = 1; i < n; i++) {
        x *= x;
    }
    return x;
}

f32 u_sqrt(f32 a){
    return sqrtf(a);
}


s8 f_compare(f32 a, f32 b){
    f32 f = a-b;
    //check for equality
    if((f<EPSILON && f>0) || (f>EPSILON && f<0)) return 0;
    //check for greatness
    if(f>0) return 1;
    
    //inferior
    return -1;
}

bool f_equal(f32 a, f32 b){
    return ABS(a - b) < EPSILON ? true : false;
}

s32 rand32(s32 min, s32 max){
    if (min >= max) {
        ERR("calling rand32() with a range of 0 (max = min). can't generate a value.")
        return -1;
    }
    // Seed the random number generator using the current time
    srand((u32)time(NULL));
    // Calculate the range of random numbers
    s32 range = max - min + 1;
    // Generate a random number within the specified range and add 'min' to it
    return (rand() % range) + min;
}