#ifndef CASTER_UTILS_H
#define CASTER_UTILS_H

#include "defines.h"

DLL_EXPORT u8 char_2_u8(char c);
DLL_EXPORT u16 char_2_u16(char c);
DLL_EXPORT u32 char_2_u32(char c);
DLL_EXPORT u64 char_2_u64(char c);

DLL_EXPORT u8 str_2_u8(const char* str);
DLL_EXPORT u16 str_2_u16(const char* str);
DLL_EXPORT u32 str_2_u32(const char* str);
DLL_EXPORT u64 str_2_u64(const char* str);
DLL_EXPORT f32 str_2_f32(const char* str);
DLL_EXPORT f64 str_2_f64(const char* str);


#endif /* CASTER_UTILS_H */