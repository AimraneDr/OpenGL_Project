#include "platform/platform.h"
#ifdef _WIN32

#include "core/inputs.h"
#include "core/events.h"
#include "core/logger.h"
#include "core/CDS/strings.h"
#include "core/CDS/dynamic_array.h"

#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>

#include <glad/glad.h>


static f64 clock_frequency;
static LARGE_INTEGER start_time;

//returns a dynamic array buffer of extensions
bool LoadOpenGL(PlatformState* state);

bool window_on_resize(u16 code, void *sender, void *listener_inst, EventContext data){
    windowConfig* listenr = (windowConfig*)listener_inst;
    listenr->width = data.data.u16[0];
    listenr->height = data.data.u16[1];
    glViewport(0,0,listenr->width,listenr->height);
    return true;
}

//proccessing a single message sent by the window handler
LRESULT CALLBACK window_message_proccess(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam)
{
switch (msg) {
        case WM_ERASEBKGND:
            // Notify the OS that erasing will be handled by the application to prevent flicker.
            return 1;
        case WM_CLOSE:
            // TODO: Fire an event for the application to quit.
            EventContext data = {};
            raise_event(EVENT_CODE_APPLICATION_QUIT, 0, data);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // Get the updated size.
            RECT r;
            GetClientRect(hwnd, &r);
            u32 width = r.right - r.left;
            u32 height = r.bottom - r.top;
            // Fire the event. The application layer should pick this up, but not handle it
            // as it shouldn be visible to other parts of the application.
            EventContext context;
            context.data.u16[0] = (u16)width;
            context.data.u16[1] = (u16)height;
            raise_event(EVENT_CODE_RESIZE, 0, context);
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // Key pressed/released
            bool pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            keys key = (u16)wParam;

            // Pass to the input subsystem for processing.
            input_process_key(key, pressed);
        } break;
        case WM_MOUSEMOVE: {
            // Mouse move
            s32 x_position = GET_X_LPARAM(lParam);
            s32 y_position = GET_Y_LPARAM(lParam);
            
            // Pass over to the input subsystem.
            input_process_mouse_move(x_position, y_position);
        } break;
        case WM_MOUSEWHEEL: {
            s32 z_delta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (z_delta != 0) {
                // Flatten the input to an OS-independent (-1, 1)
                z_delta = (z_delta < 0) ? -1 : 1;
                input_process_mouse_wheel(z_delta);
            }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            bool pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            buttons mouse_button = BUTTON_MAX_BUTTONS;
            switch (msg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    mouse_button = BUTTON_LEFT;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    mouse_button = BUTTON_MIDDLE;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    mouse_button = BUTTON_RIGHT;
                    break;
            }

            // Pass over to the input subsystem.
            if (mouse_button != BUTTON_MAX_BUTTONS) {
                input_process_button(mouse_button, pressed);
            }
        } break;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

bool platform_start(windowConfig* wconfig, PlatformState* out_state)
{
    const char* CLASS_NAME = "gameWindowClass";
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = window_message_proccess;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClass(&wc);
    
    // Step 2: Create the window
    out_state->hwnd = CreateWindowEx(
        0,                      // Optional window styles
        CLASS_NAME,             // Window class name
        wconfig->name,            // Window title
        WS_OVERLAPPEDWINDOW | CS_OWNDC,    // Window style
    
        // position
        wconfig->x, wconfig->y,
        // size
        wconfig->width, wconfig->height,
    
        NULL,                   // Parent window    
        NULL,                   // Menu
        GetModuleHandle(NULL),  // Instance handle
        NULL                    // Additional application data
        );
    
    if (out_state->hwnd == NULL)
    {
        FATAL("failed to create window!")
        return false;
    }

    out_state->dc = GetDC(out_state->hwnd);

    if (out_state->dc == NULL)
    {
        FATAL("Failed to get device context (HDC)!")
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    u32 pixelFormat = ChoosePixelFormat(out_state->dc,&pfd);
    if(pixelFormat == 0){
        FATAL("could not set the pixel format for the device context.");
        return false;
    }else{
        SetPixelFormat(out_state->dc,pixelFormat,&pfd);
    }

    out_state->rc = wglCreateContext(out_state->dc);
    if(out_state->rc == NULL){
        FATAL("Failed to create OpenGl Context");
        return false;
    }

    if(!wglMakeCurrent(out_state->dc, out_state->rc)){
        FATAL("Failed to set current openGL context!");
        return false;
    }

    if(!LoadOpenGL(out_state)){
        FATAL("Failed to load OpenGL!");
    }


    if(ShowWindow(out_state->hwnd, SW_SHOW)){
        ERR("Failed to Present application Window|");
        return false;
    }
    
    // Clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);


    //add resize event Listner
    DEBUG("before adding listner")
    event_add_listner(EVENT_CODE_RESIZE, wconfig, window_on_resize);
    DEBUG("After adding listner")

    INFO("platform system initialized.");
    return true;
}

bool platform_shutdown(PlatformState* state){
    ReleaseDC(state->hwnd, state->dc);
    wglDeleteContext(state->rc);
    return true;
}

bool LoadOpenGL(PlatformState* state){
    if(!gladLoadGL()){
        FATAL("Fialed to load OpenGL");
        return false;
    }
    DEBUG("openGL version : %s", glGetString(GL_VERSION));
    glViewport(0,0,state->wconfig.width,state->wconfig.height);
    return true;
}

bool platform_window_proccess_msg(){
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}


void platform_console_write(const char *message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    static u32 levels[6] = {15 | BACKGROUND_RED, FOREGROUND_RED, 6, 2,  11 | (BACKGROUND_INTENSITY << 4), 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char *message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    static u32 levels[6] = {15 | BACKGROUND_RED, FOREGROUND_RED, 6, 2,  11 | (BACKGROUND_INTENSITY << 4), 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time() {
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}


#endif /*WIN32*/