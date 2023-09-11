#ifdef __linux__
#define _POSIX_C_SOURCE 200809L  // Define the version of POSIX you want to use

#include "platform/platform.h"

#include "core/inputs.h"
#include "core/events.h"
#include "core/logger.h"
#include "core/CDS/strings.h"
#include "core/CDS/dynamic_array.h"

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
// #include <GL/gl.h>
#include <glad/glad.h>
#include <GL/glx.h>

#include <time.h>

#include <stdlib.h>
#include <stdio.h>

// Define ANSI escape codes for text color
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_INTENSITY "\x1b[1m"
#define ANSI_BACKGROUND_RED "\x1b[41m"


static struct timespec frequency;
static struct timespec start_time;

static keys translate_keycode(u32 x_keycode);


bool window_on_resize(u16 code, void *sender, void *listener_inst, EventContext data){
    WindowState* listenr = (WindowState*)listener_inst;
    listenr->width = data.data.u16[0];
    listenr->height = data.data.u16[1];
    glViewport(0,0,listenr->width,listenr->height);
    return true;
}

bool LoadOpenGL(PlatformState* state){
    if(!gladLoadGL()){
        FATAL("Fialed to load OpenGL");
        return false;
    }
    DEBUG("openGL version : %s", glGetString(GL_VERSION));
    glViewport(0,0,state->w_state.width,state->w_state.height);
    return true;
}

bool platform_start(windowConfig wconfig, PlatformState* out_state)
{
    out_state->w_state.name = wconfig.name;
    out_state->w_state.width = wconfig.width;
    out_state->w_state.height = wconfig.height;
    out_state->w_state.x = wconfig.x;
    out_state->w_state.y = wconfig.y;
    out_state->w_state.resizable = wconfig.resizable;

    out_state->w_state.display = XOpenDisplay(NULL);
    if (out_state->w_state.display == NULL) {
        ERR("Cannot open display");
        return 1;
    }

    // Create OpenGL context
    int attribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(out_state->w_state.display, 0, attribs);
    if (vi == NULL) {
        ERR("No suitable visual found");
        return 1;
    }
    
    GLXContext context = glXCreateContext(out_state->w_state.display, vi, NULL, GL_TRUE);
    if (context == NULL) {
        ERR("Failed to create OpenGL context");
        return 1;
    }
    out_state->w_state.gl_context = (void*)context;

    // Create a window
    out_state->w_state.window = XCreateSimpleWindow(out_state->w_state.display, RootWindow(out_state->w_state.display, 0), wconfig.x, wconfig.y, wconfig.width, wconfig.height, 1, BlackPixel(out_state->w_state.display, 0), WhitePixel(out_state->w_state.display, 0));
    XSelectInput(
        out_state->w_state.display, out_state->w_state.window, 
        KeyPressMask |
        KeyReleaseMask |
        ButtonPressMask |
        ButtonReleaseMask |
        ButtonMotionMask
    );

    XkbQueryExtension(out_state->w_state.display, NULL, NULL, NULL, NULL, NULL);
    XkbSelectEvents(out_state->w_state.display, XkbUseCoreKbd, 0,0);


    XMapWindow(out_state->w_state.display, out_state->w_state.window);

    // Bind OpenGL context to the window
    glXMakeCurrent(out_state->w_state.display, out_state->w_state.window, (GLXContext)context);

    if(!LoadOpenGL(out_state)){
        FATAL("Failed to load openGL");
        return 1;
    }


    // Clock setup
    clock_getres(CLOCK_MONOTONIC, &frequency);
    double clock_frequency = 1.0 / (double)frequency.tv_nsec;
    clock_gettime(CLOCK_MONOTONIC, &start_time);


    //add resize event Listner
    event_add_listner(EVENT_CODE_RESIZE, &out_state->w_state, window_on_resize);

    INFO("platform system initialized.");
    return true;
}

bool platform_shutdown(PlatformState* state){
    // Cleanup
    glXMakeCurrent(state->w_state.display, None, NULL);
    glXDestroyContext(state->w_state.display, state->w_state.gl_context);
    XDestroyWindow(state->w_state.display, state->w_state.window);
    XCloseDisplay(state->w_state.display);
    return true;
}


bool platform_window_proccess_events(WindowState* state){
    
    while (XPending(state->display))
    {
        XNextEvent(state->display, &state->event);
        switch (state->event.type)
        {
            case DestroyNotify: {
                //Quit
                EventContext data = {};
                raise_event(EVENT_CODE_APPLICATION_QUIT, 0, data);
            }
                break;
            case ConfigureNotify:{
                //Resize
                // Get the updated size.
                u32 width = state->event.xconfigure.width;
                u32 height = state->event.xconfigure.height;
                // Fire the event. The application layer should pick this up, but not handle it
                // as it shouldn be visible to other parts of the application.
                EventContext context;
                context.data.u16[0] = (u16)width;
                context.data.u16[1] = (u16)height;
                raise_event(EVENT_CODE_RESIZE, 0, context);
            }
                break;
            case KeyPress:{
                KeySym keysym = XkbKeycodeToKeysym(state->display, state->event.xkey.keycode, 0, 0);
                keys key = translate_keycode(keysym);
                DEBUG("key pressed : %d", key)
                input_process_key(key, true);
            }
                break;
            case KeyRelease:{
                KeySym keysym = XkbKeycodeToKeysym(state->display, state->event.xkey.keycode, 0, 0);
                keys key = translate_keycode(keysym);
                input_process_key(key, false);
            }
                break;
            case ButtonPress:
            case ButtonRelease:{
                bool pressed = state->event.type == ButtonPress ? true : false;
                
                buttons mouse_button = BUTTON_MAX_BUTTONS;
                u32 xbtn = state->event.xbutton.button;

                if (xbtn == Button1)        mouse_button = BUTTON_LEFT;
                else if(xbtn == Button2)    mouse_button = BUTTON_MIDDLE;
                else if(xbtn == Button3)    mouse_button = BUTTON_RIGHT;
                
                //if mouse btn is decided break
                if (mouse_button != BUTTON_MAX_BUTTONS) {
                    input_process_button(mouse_button, pressed);
                    break;
                }

                //if not check for scrolling
                s32 z_delta = 0;
                // Flatten the input to an OS-independent (-1, 1)
                if (xbtn == Button4){
                    //scroll up
                    z_delta = 1;
                }
                else if(xbtn == Button5){
                    //scroll down
                    z_delta = -1;
                }

                if(z_delta != 0) input_process_mouse_wheel(z_delta);
            }
                break;
            case MotionNotify:{
                //mouse movement
                s16 xPos = state->event.xmotion.x; 
                s16 yPos = state->event.xmotion.y;

                input_process_mouse_move(xPos, yPos);
            }
                break;
            default:
                return false;
        }
    }
    return true;
}




void platform_console_write(const char *message, u8 colour) {
    // Define ANSI color codes based on the 'colour' parameter
    const char *ansi_color;
    switch (colour) {
        case 0: // FATAL (Red text, white background, bold)
            ansi_color = ANSI_COLOR_INTENSITY ANSI_COLOR_RED ANSI_BACKGROUND_RED;
            break;
        case 1: // ERROR (Red text)
            ansi_color = ANSI_COLOR_RED;
            break;
        case 2: // WARN (Yellow text)
            ansi_color = ANSI_COLOR_YELLOW;
            break;
        case 3: // INFO (Green text)
            ansi_color = ANSI_COLOR_GREEN;
            break;
        case 4: // DEBUG (Cyan text)
            ansi_color = ANSI_COLOR_CYAN;
            break;
        case 5: // TRACE (White text)
            ansi_color = ANSI_COLOR_WHITE;
            break;
        default:
            ansi_color = ANSI_COLOR_RESET; // Default color (reset)
    }

    // Print the message with the specified color
    printf("%s%s%s", ansi_color, message, ANSI_COLOR_RESET);

    // Flush stdout to ensure the message is displayed immediately
    fflush(stdout);
}

void platform_console_write_error(const char *message, u8 colour) {
    // Use ANSI escape codes to set the text color to red (or other colors as needed)
    const char *ansi_color = ANSI_COLOR_RED;

    // Print the error message with the specified color to stderr
    fprintf(stderr, "%s%s%s", ansi_color, message, ANSI_COLOR_RESET);

    // Flush stderr to ensure the error message is immediately displayed
    fflush(stderr);
}

f64 platform_get_absolute_time() {
    struct timespec now_time;
    clock_gettime(CLOCK_MONOTONIC, &now_time);

    // Convert seconds and nanoseconds to a floating-point timestamp
    return (f64)now_time.tv_sec + (f64)now_time.tv_nsec * 1e-9;
}

// Key translation
static keys translate_keycode(u32 x_keycode) {
    switch (x_keycode) {
        case XK_BackSpace:
            return KEY_BACKSPACE;
        case XK_Return:
            return KEY_ENTER;
        case XK_Tab:
            return KEY_TAB;
            // case XK_Shift: return KEY_SHIFT;
            // case XK_Control: return KEY_CONTROL;

        case XK_Pause:
            return KEY_PAUSE;
        case XK_Caps_Lock:
            return KEY_CAPITAL;

        case XK_Escape:
            return KEY_ESCAPE;

            // Not supported
            // case : return KEY_CONVERT;
            // case : return KEY_NONCONVERT;
            // case : return KEY_ACCEPT;

        case XK_Mode_switch:
            return KEY_MODECHANGE;

        case XK_space:
            return KEY_SPACE;
        case XK_Prior:
            return KEY_PAGEUP;
        case XK_Next:
            return KEY_PAGEDOWN;
        case XK_End:
            return KEY_END;
        case XK_Home:
            return KEY_HOME;
        case XK_Left:
            return KEY_LEFT;
        case XK_Up:
            return KEY_UP;
        case XK_Right:
            return KEY_RIGHT;
        case XK_Down:
            return KEY_DOWN;
        case XK_Select:
            return KEY_SELECT;
        case XK_Print:
            return KEY_PRINT;
        case XK_Execute:
            return KEY_EXECUTE;
        // case XK_snapshot: return KEY_SNAPSHOT; // not supported
        case XK_Insert:
            return KEY_INSERT;
        case XK_Delete:
            return KEY_DELETE;
        case XK_Help:
            return KEY_HELP;

        case XK_Meta_L:
            return KEY_LSUPER;  // TODO: not sure this is right
        case XK_Meta_R:
            return KEY_RSUPER;
            // case XK_apps: return KEY_APPS; // not supported

            // case XK_sleep: return KEY_SLEEP; //not supported

        case XK_KP_0:
            return KEY_NUMPAD0;
        case XK_KP_1:
            return KEY_NUMPAD1;
        case XK_KP_2:
            return KEY_NUMPAD2;
        case XK_KP_3:
            return KEY_NUMPAD3;
        case XK_KP_4:
            return KEY_NUMPAD4;
        case XK_KP_5:
            return KEY_NUMPAD5;
        case XK_KP_6:
            return KEY_NUMPAD6;
        case XK_KP_7:
            return KEY_NUMPAD7;
        case XK_KP_8:
            return KEY_NUMPAD8;
        case XK_KP_9:
            return KEY_NUMPAD9;
        case XK_multiply:
            return KEY_MULTIPLY;
        case XK_KP_Add:
            return KEY_ADD;
        case XK_KP_Separator:
            return KEY_SEPARATOR;
        case XK_KP_Subtract:
            return KEY_SUBTRACT;
        case XK_KP_Decimal:
            return KEY_DECIMAL;
        case XK_KP_Divide:
            return KEY_DIVIDE;
        case XK_F1:
            return KEY_F1;
        case XK_F2:
            return KEY_F2;
        case XK_F3:
            return KEY_F3;
        case XK_F4:
            return KEY_F4;
        case XK_F5:
            return KEY_F5;
        case XK_F6:
            return KEY_F6;
        case XK_F7:
            return KEY_F7;
        case XK_F8:
            return KEY_F8;
        case XK_F9:
            return KEY_F9;
        case XK_F10:
            return KEY_F10;
        case XK_F11:
            return KEY_F11;
        case XK_F12:
            return KEY_F12;
        case XK_F13:
            return KEY_F13;
        case XK_F14:
            return KEY_F14;
        case XK_F15:
            return KEY_F15;
        case XK_F16:
            return KEY_F16;
        case XK_F17:
            return KEY_F17;
        case XK_F18:
            return KEY_F18;
        case XK_F19:
            return KEY_F19;
        case XK_F20:
            return KEY_F20;
        case XK_F21:
            return KEY_F21;
        case XK_F22:
            return KEY_F22;
        case XK_F23:
            return KEY_F23;
        case XK_F24:
            return KEY_F24;

        case XK_Num_Lock:
            return KEY_NUMLOCK;
        case XK_Scroll_Lock:
            return KEY_SCROLL;

        case XK_KP_Equal:
            return KEY_NUMPAD_EQUAL;

        case XK_Shift_L:
            return KEY_LSHIFT;
        case XK_Shift_R:
            return KEY_RSHIFT;
        case XK_Control_L:
            return KEY_LCONTROL;
        case XK_Control_R:
            return KEY_RCONTROL;
        case XK_Alt_L:
            return KEY_LALT;
        case XK_Alt_R:
            return KEY_RALT;

        case XK_semicolon:
            return KEY_SEMICOLON;
        case XK_plus:
            return KEY_EQUAL;
        case XK_comma:
            return KEY_COMMA;
        case XK_minus:
            return KEY_MINUS;
        case XK_period:
            return KEY_PERIOD;
        case XK_slash:
            return KEY_SLASH;
        case XK_grave:
            return KEY_GRAVE;

        case XK_0:
            return KEY_0;
        case XK_1:
            return KEY_1;
        case XK_2:
            return KEY_2;
        case XK_3:
            return KEY_3;
        case XK_4:
            return KEY_4;
        case XK_5:
            return KEY_5;
        case XK_6:
            return KEY_6;
        case XK_7:
            return KEY_7;
        case XK_8:
            return KEY_8;
        case XK_9:
            return KEY_9;

        case XK_a:
        case XK_A:
            return KEY_A;
        case XK_b:
        case XK_B:
            return KEY_B;
        case XK_c:
        case XK_C:
            return KEY_C;
        case XK_d:
        case XK_D:
            return KEY_D;
        case XK_e:
        case XK_E:
            return KEY_E;
        case XK_f:
        case XK_F:
            return KEY_F;
        case XK_g:
        case XK_G:
            return KEY_G;
        case XK_h:
        case XK_H:
            return KEY_H;
        case XK_i:
        case XK_I:
            return KEY_I;
        case XK_j:
        case XK_J:
            return KEY_J;
        case XK_k:
        case XK_K:
            return KEY_K;
        case XK_l:
        case XK_L:
            return KEY_L;
        case XK_m:
        case XK_M:
            return KEY_M;
        case XK_n:
        case XK_N:
            return KEY_N;
        case XK_o:
        case XK_O:
            return KEY_O;
        case XK_p:
        case XK_P:
            return KEY_P;
        case XK_q:
        case XK_Q:
            return KEY_Q;
        case XK_r:
        case XK_R:
            return KEY_R;
        case XK_s:
        case XK_S:
            return KEY_S;
        case XK_t:
        case XK_T:
            return KEY_T;
        case XK_u:
        case XK_U:
            return KEY_U;
        case XK_v:
        case XK_V:
            return KEY_V;
        case XK_w:
        case XK_W:
            return KEY_W;
        case XK_x:
        case XK_X:
            return KEY_X;
        case XK_y:
        case XK_Y:
            return KEY_Y;
        case XK_z:
        case XK_Z:
            return KEY_Z;

        default:
            return 0;
    }
}

#endif /* __linux */
//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