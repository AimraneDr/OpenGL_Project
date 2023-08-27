#ifndef PLATFORM_H
#define PLATFORM_H

#include "defines.h"
#include "Windows.h"
#include "engine_types.h"



DLL_EXPORT bool platform_start(windowConfig* wconfig, PlatformState* outstate);
DLL_EXPORT bool platform_shutdown(PlatformState* state);

bool platform_window_proccess_msg();

void platform_console_write(const char* msg, u8 color);
void platform_console_write_error(const char* msg, u8 color);

f64 platform_get_absolute_time();

#endif /* PLATFORM_H */