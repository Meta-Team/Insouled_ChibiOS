//
// Created by liuzikai on 2018/6/2.
//

#include "debug.h"

#include "global.h"

#include "led.h"
#include "serial_debug.h"

#include "remote.h"
#include "can.h"

#include "chassis.h"
#include "gimbal.h"
#include "mode_handle.h"

static THD_WORKING_AREA(debug_wa, 256);


static THD_FUNCTION(debug_thread, p) {
    (void) p;
    chRegSetThreadName("debug_thread");

    while (true) {
        chThdSleepMilliseconds(500);
    }
}

void debug_thread_init(void) {
    chThdCreateStatic(debug_wa, sizeof(debug_wa), LOWPRIO,
                      debug_thread, NULL);
}