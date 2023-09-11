#ifndef PLATFORM_DEFINES_H
#define PLATFORM_DEFINES_H

#ifdef WIN32
// #include <Windows.h>
#endif /* WIN32 */
#ifdef __linux
#include <X11/Xlib.h>
#endif /* __linux */

typedef struct WindowState{
#ifdef WIN32
    HINSTANCE h_instance;
    HWND hwnd;
    HDC dc;
    HGLRC rc;
#endif /* WIN32 */
#ifdef __linux
    Display *display;
    Window window;
    XEvent event;
    void* gl_context;
#endif /* __linux */

    char* name;
    u16 width, height;
    u32 x, y;
    bool resizable;
}WindowState;

#endif /* PLATFORM_DEFINES_H */