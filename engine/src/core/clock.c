#include "core/clock.h"

#include "platform/platform.h"
#include "core/logger.h"
#include "core/CDS/strings.h"
void clock_update(Clock* clock) {
    if (clock->start_time != 0) {
        clock->preElapsed = clock->elapsed; 
        clock->elapsed = platform_get_absolute_time() - clock->start_time;
    }
}

void clock_start(Clock* clock) {
    clock->start_time = platform_get_absolute_time();
    clock->elapsed = 0;
    clock->preElapsed = 0;
}

void clock_stop(Clock* clock) {
    clock->start_time = 0;
}