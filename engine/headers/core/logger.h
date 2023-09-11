#ifndef LOGGER_H
#define LOGGER_H

#include "defines.h"

typedef enum{
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_TRACE
}logLevel;

bool logger_sys_init();
DLL_EXPORT void logger_log(logLevel lv, const char* msg, ...);

#ifdef _DEBUG
    #ifndef TRACE
        #define TRACE(...) logger_log(LOG_TRACE, __VA_ARGS__);
    #endif /*TRACE*/
    #ifndef DEBUG
        #define DEBUG(...) logger_log(LOG_DEBUG, __VA_ARGS__);
    #endif /*DEBUG*/
#else
    #define TRACE(...)
    #define DEBUG(...)
#endif /* _DEBUG */
#ifndef INFO
    #define INFO(...) logger_log(LOG_INFO, __VA_ARGS__);
#endif /*INFO*/
#ifndef WARN
    #define WARN(...) logger_log(LOG_WARN, __VA_ARGS__);
#endif /*WARN*/
#ifndef ERR
    #define ERR(...) logger_log(LOG_ERROR, __VA_ARGS__);
#endif /*ERROR*/
#ifndef FATAL
    #define FATAL(...) logger_log(LOG_FATAL, __VA_ARGS__);
#endif /*FATAL*/


#endif /* LOGGER_H */