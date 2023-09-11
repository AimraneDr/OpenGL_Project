#ifndef EVENTS_H
#define EVENTS_H

#include "defines.h"

///NOTE: most of this code is from a guy named Travis from a game engine dev tutorial (Kohe game engine)
///      I tried to changge a little bit to make it my own but i kept most of it as it is 'cause the code is so good. 


typedef struct EventContext{
    // 128 bytes
    union {
        s64 s64[2];
        u64 u64[2];
        f64 f64[2];

        s32 s32[4];
        u32 u32[4];
        f32 f32[4];

        s16 s16[8];
        u16 u16[8];

        s8 s8[16];
        u8 u8[16];

        char c[16];
    } data;

}EventContext;

typedef bool (*EventCallBack)(u16 code, void* sender, void* listener_inst, EventContext data);


bool events_sys_init();
void events_sys_shutdown();

/**
 * Register to listen for when events are sent with the provided code. Events with duplicate
 * listener/callback combos will not be registered again and will cause this to return FALSE.
 * @param code The event code to listen for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param on_event The callback function pointer to be invoked when the event code is fired.
 * @returns TRUE if the event is successfully registered; otherwise false.
 */
DLL_EXPORT bool event_add_listner(u16 code, void* listener, EventCallBack callBack);

/**
 * Unregister from listening for when events are sent with the provided code. If no matching
 * registration is found, this function returns FALSE.
 * @param code The event code to stop listening for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param on_event The callback function pointer to be unregistered.
 * @returns TRUE if the event is successfully unregistered; otherwise false.
 */
DLL_EXPORT bool event_remove_listner(u16 code, void* listener, EventCallBack callBack);

/**
 * Raise an event to listeners of the given code. If an event handler returns 
 * TRUE, the event is considered handled and is not passed on to any more listeners.
 * @param code The event code to fire.
 * @param sender A pointer to the sender. Can be 0/NULL.
 * @param data The event data.
 * @returns TRUE if handled, otherwise FALSE.
 */
DLL_EXPORT bool raise_event(u16 code, void* sender, EventContext context);


// System internal event codes. Application should use codes beyond 255.
typedef enum EventCode {
    // Shuts the application down on the next frame.
    EVENT_CODE_APPLICATION_QUIT = 0x01,

    // Keyboard key pressed.
    /* Context usage:
     * u16 key_code = data.data.u16[0];
     */
    EVENT_CODE_KEY_DOWN = 0x02,

    // Keyboard key released.
    /* Context usage:
     * u16 key_code = data.data.u16[0];
     */
    EVENT_CODE_KEY_UP = 0x03,

    // Mouse button pressed.
    /* Context usage:
     * u16 button = data.data.u16[0];
     */
    EVENT_CODE_BUTTON_DOWN = 0x04,

    // Mouse button released.
    /* Context usage:
     * u16 button = data.data.u16[0];
     */
    EVENT_CODE_BUTTON_UP = 0x05,

    // Mouse moved.
    /* Context usage:
     * u16 x = data.data.u16[0];
     * u16 y = data.data.u16[1];
     */
    EVENT_CODE_MOUSE_MOVE = 0x06,

    // Mouse moved.
    /* Context usage:
     * u8 z_delta = data.data.u8[0];
     */
    EVENT_CODE_MOUSE_WHEEL = 0x07,

    // Resized/resolution changed from the OS.
    /* Context usage:
     * u16 width = data.data.u16[0];
     * u16 height = data.data.u16[1];
     */
    EVENT_CODE_RESIZE = 0x08,

    MAX_EVENT_CODE = 0xFF
} system_event_code;

#endif /* EVENTS_H */