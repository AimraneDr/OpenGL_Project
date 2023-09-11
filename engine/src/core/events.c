#include "core/events.h"

#include "core/logger.h"
#include "core/CDS/dynamic_array.h"
#include <string.h>

typedef struct registered_event {
    void* listener;
    EventCallBack callback;
} registered_event;

typedef struct event_code_entry {
    registered_event* events;
} event_code_entry;

// This should be more than enough codes...
#define MAX_MESSAGE_CODES 16384

// State structure.
typedef struct event_system_state {
    // Lookup table for event codes.
    event_code_entry registered[MAX_MESSAGE_CODES];
} event_system_state;

/**
 * Event system internal state.
 */
static bool is_initialized = false;
static event_system_state state;

bool events_sys_init() {
    if (is_initialized == true) {
        ERR("failed to initialize events subsystem.");
        return false;
    }
    is_initialized = false;
    memset(&state, 0, sizeof(state));

    is_initialized = true;
    INFO("events subsystem initialized.");
    return true;
}

void events_sys_shutdown() {
    // Free the events arrays. And objects pointed to should be destroyed on their own.
    for(u16 i = 0; i < MAX_MESSAGE_CODES; ++i){
        if(state.registered[i].events != 0) {
            dynamic_array_destroy(state.registered[i].events);
            state.registered[i].events = 0;
        }
    }
}

bool event_add_listner(u16 code, void* listener, EventCallBack callBack) {
    if(is_initialized == false) {
        return false;
    }

    if(state.registered[code].events == 0) {
        state.registered[code].events = dynamic_array_create(registered_event);
    }

    u64 registered_count = dynamic_array_length(state.registered[code].events);
    for(u64 i = 0; i < registered_count; ++i) {
        if(state.registered[code].events[i].listener == listener) {
            // TODO: warn
            return false;
        }
    }

    // If at this point, no duplicate was found. Proceed with registration.
    registered_event event;
    event.listener = listener;
    event.callback = callBack;
    dynamic_array_push(state.registered[code].events, event);
    return true;
}

bool event_remove_listner(u16 code, void* listener, EventCallBack callBack) {
    if(is_initialized == false) {
        return false;
    }

    // On nothing is registered for the code, boot out.
    if(state.registered[code].events == 0) {
        // TODO: warn
        return false;
    }

    u64 registered_count = dynamic_array_length(state.registered[code].events);
    for(u64 i = 0; i < registered_count; ++i) {
        registered_event e = state.registered[code].events[i];
        if(e.listener == listener && e.callback == callBack) {
            // Found one, remove it
            registered_event popped_event;
            dynamic_array_pop_at(state.registered[code].events, i, &popped_event);
            return true;
        }
    }

    // Not found.
    return false;
}

bool raise_event(u16 code, void* sender, EventContext context) {
    if(is_initialized == false) {
        return false;
    }

    // If nothing is registered for the code, boot out.
    if(state.registered[code].events == 0) {
        return false;
    }

    u64 registered_count = dynamic_array_length(state.registered[code].events);
    for(u64 i = 0; i < registered_count; ++i) {
        registered_event e = state.registered[code].events[i];
        if(e.callback(code, sender, e.listener, context)) {
            // Message has been handled, do not send to other listeners.
            // return true;
        }
    }

    return true;
}