#include "utils/caster.h"

#include <stdlib.h>

u8  char_2_u8 (char c){
    if(c >= '0' && c <= '9'){
        u8 r = c -'0';
        return r;
    }
    return 0;
}
u16 char_2_u16(char c){
    return (u16)char_2_u8(c);
}
u32 char_2_u32(char c){
    return (u32)char_2_u8(c);
}
u64 char_2_u64(char c){
    return (u64)char_2_u8(c);
}

u8  str_2_u8 (const char* str){
    return (u8)str_2_u32(str);
}
u16 str_2_u16(const char* str){
    return (u16)str_2_u32(str);
}
u32 str_2_u32(const char* str){
    return (u32)str_2_u64(str);
}

u64 str_2_u64(const char* str){
    u64 result = 0;
    u32 i = 0;
    u32 factor = 1;

    // Check for a minus sign at the beginning
    if (str[0] == '-') {
        factor = -1;
        i = 1;  // Skip the minus sign
    }

    // Iterate through each character of the string
    for (u32 i = 0; str[i] != '\0'; i++) {
        if(str[i] == ' ' || str[i] == '\r') continue;
        // Check if the character is a digit
        if (str[i] >= '0' && str[i] <= '9') {
            // Multiply the result by 10 and add the digit value
            result = result * 10 + (str[i] - '0');
        }else if(str[i] == '\n') return result;
        else {
            // Invalid character found, handle error or return a special value
            // In this example, we simply break the loop and return the current result
            return 0;
        }
    }
    return result * factor;
    
}
f32 str_2_f32(const char* str){
    f32 result = strtof(str, NULL);
    return result;
}
f64 str_2_f64(const char* str){
    f64 result = strtod(str, NULL);
    return result;
}