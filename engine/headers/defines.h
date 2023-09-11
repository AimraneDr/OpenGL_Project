#ifndef DEFINES_H
#define DEFINES_H

//unsigned integers
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

//signed integers
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

//floating points
typedef float f32;
typedef double f64;

typedef char bool;
#define true 1
#define false 0

typedef enum RESULT_MSG{
    RESULT_MSG_FALSE = 0,
    RESULT_MSG_UNIDENTIFIED_ERROR,
    RESULT_MSG_NON_EXISTING_ENTITY,
    RESULT_MSG_MESSING_COMPONENT,
    RESULT_MSG_SUCCESS
}RESULT_MSG;

#define null (void*)0

#ifdef API
    #ifdef _WIN32
        #define DLL_EXPORT __declspec(dllexport)
    #elif defined(__linux__)
        #define DLL_EXPORT __attribute__((visibility("default")))
    #endif
#else
    #define DLL_EXPORT
#endif

#define TYPE_NAME(type) #type

#endif /* DEFINES_H */
