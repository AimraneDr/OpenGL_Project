#include "core/logger.h"

#include "core/CDS/strings.h"
#include "platform/platform.h"

#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#elif defined(__linux__)
#include <sys/time.h>
#endif

bool logger_sys_init(){
    INFO("logging subsystem initialized.");
    return true;
}

// void logger_log(logLevel lv, const char* msg){
//     const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN] :  ", "[INFO] :  ", "[DEBUG]: ", "[TRACE]: "};
//     bool is_error = lv < LOG_WARN;

//     // Get current time
//     SYSTEMTIME st;
//     GetLocalTime(&st);

//     // Format the time as "h:min:sec:ms"
//     char timeStr[13]; // 12 characters: "hh:mm:ss:ms\0"
//     sprintf(timeStr, "%.2d:%.2d:%.2d:%.3d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);


//     const char* out_msg = stringFormat("[%s] - %s%s\n",timeStr, level_strings[lv], msg);

//     // Platform-specific output.
//     if (is_error) {
//         platform_console_write_error(out_msg, lv);
//     } else {
//         platform_console_write(out_msg, lv);
//     }
// }

void logger_log(logLevel lv, const char* msg, ...) {
    const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN] :  ", "[INFO] :  ", "[DEBUG]: ", "[TRACE]: "};
    bool is_error = lv < LOG_WARN;

    char timeStr[13]; // 12 characters: "hh:mm:ss:ms\0"
#ifdef WIN32
    // Get current time
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Format the time as "h:min:sec:ms"
    sprintf(timeStr, "%.2d:%.2d:%.2d:%.3d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#elif defined(__linux__)
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);

    // Calculate hours, minutes, seconds, and milliseconds
    int hours = currentTime.tv_sec / 3600 % 24;
    int minutes = (currentTime.tv_sec / 60) % 60;
    int seconds = currentTime.tv_sec % 60;
    int milliseconds = currentTime.tv_usec / 1000;

    // Format the time as "h:min:sec:ms"
    sprintf(timeStr, "%.2d:%.2d:%.2d:%.3d", hours, minutes, seconds, milliseconds);

#endif
    // Format the log message
    __builtin_va_list args;
    __builtin_va_start(args, msg);

    // Determine the size of the formatted string
    __builtin_va_list args_copy;
    __builtin_va_copy(args_copy, args);
    int size = vsnprintf(NULL, 0, msg, args_copy);
    __builtin_va_end(args_copy);

    // Allocate memory for the formatted string
    char* buffer = (char*)malloc((size + 1) * sizeof(char));

    // Format the string
    vsprintf(buffer, msg, args);

    __builtin_va_end(args);

    char* out_msg = stringFormat("[%s] - %s%s\n", timeStr, level_strings[lv], buffer);

    // Platform-specific output.
    if (is_error) {
        platform_console_write_error(out_msg, lv);
    } else {
        platform_console_write(out_msg, lv);
    }

    // Free the dynamically allocated memory
    free(buffer);
    free(out_msg);
}


