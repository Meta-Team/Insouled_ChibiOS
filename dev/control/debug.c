//
// Created by liuzikai on 2018/6/2.
//

#include <info_interaction/remote.h>
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

#ifdef DEBUG_CHASSIS_PID
int chassis_pid_operand; //kp: 0; ki: 1; kd: 2, i_limit: 3, out_limit: 4.
#endif

static THD_FUNCTION(debug_thread, p) {
    (void) p;
    chRegSetThreadName("debug_thread");

    while (true) {
#ifdef DEBUG_CHASSIS_PID
        if (keyboard.press_z) chassis_pid_operand = 0;
        else if (keyboard.press_x) chassis_pid_operand = 1;
        else if (keyboard.press_c) chassis_pid_operand = 2;
        else if (keyboard.press_v) chassis_pid_operand = 3;
        else if (keyboard.press_b) chassis_pid_operand = 4;

        float delta = 0.0f;

        if (keyboard.press_r) delta = 0.1f;
        else if (keyboard.press_f) delta = -0.1f;

        if (chassis_pid_operand == 3 || chassis_pid_operand == 4) delta *= 1000.0f;

        if (keyboard.press_shift) delta *= 5.0f;
        else if (keyboard.press_ctrl) delta *= 0.1f;

        if (!EQUAL_ZERO(delta)) {
            chassis_debug_change_pid_parameters(chassis_pid_operand, delta);
        }

        chassis_debug_print_pid_parameters(chassis_pid_operand);
#endif
        chThdSleepMilliseconds(200);
    }
}

void debug_thread_init(void) {
#ifdef DEBUG_CHASSIS_PID
    chassis_pid_operand = 0;
#endif
    chThdCreateStatic(debug_wa, sizeof(debug_wa), LOWPRIO,
                      debug_thread, NULL);
}