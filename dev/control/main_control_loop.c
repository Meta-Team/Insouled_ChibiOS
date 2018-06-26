//
// Created by liuzikai on 2018/6/1.
//

#include "main_control_loop.h"

void main_control_loop(void) {

    switch (global_mode) {
        case GLOBAL_MODE_REMOTE_CHASSIS:
            GIMBAL_ZERO_CURRENT();
            chassis_calc_remote();

            break;
        case GLOBAL_MODE_REMOTE_GIMBAL:
            CHASSIS_ZERO_CURRENT();
            gimbal_calc_remote();
            break;
        case GLOBAL_MODE_PC:
            gimbal_calc_pc();
            chassis_calc_pc();
            me_arm_handle();
            break;
        case GLOBAL_MODE_REMOTE_ME_ARM:
            gimbal_calc_hold();
            CHASSIS_ZERO_CURRENT();
            me_arm_handle();
            break;
        default:
            CHASSIS_ZERO_CURRENT();
            GIMBAL_ZERO_CURRENT();
    }

    send_chassis_currents();
    send_gimbal_shoot_currents();
}

/**
 * Thread to handle modes.
 * @workarea 256
 * @priority High
 * @interval 100ms
 */

static THD_WORKING_AREA(mode_handle_wa, 256);

static THD_FUNCTION(mode_handle, p) {
    (void) p;
    chRegSetThreadName("mode_handle");

    while (true) {
        handle_modes();
        if (global_mode == GLOBAL_MODE_SAFETY) {
            CHASSIS_ZERO_CURRENT();
            GIMBAL_ZERO_CURRENT();
            send_chassis_currents();
            send_gimbal_shoot_currents();
        }
        chThdSleepMilliseconds(100);
    }
}

void mode_handle_thread_init(void) {
    chThdCreateStatic(mode_handle_wa, sizeof(mode_handle_wa), HIGHPRIO,
                      mode_handle, NULL);
}