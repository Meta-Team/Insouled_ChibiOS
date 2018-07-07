//
// Created by liuzikai on 2018/6/1.
//

#include "main_control_loop.h"

void main_control_loop(void) {

    switch (global_mode) {
        case GLOBAL_MODE_REMOTE_CHASSIS:
            GIMBAL_ZERO_CURRENT();
            chassis_calculate();
            SHOOT_ZERO_CURRENT();

            break;
        case GLOBAL_MODE_REMOTE_GIMBAL:
            CHASSIS_ZERO_CURRENT();
            gimbal_calculate();
            shoot_calculate();
            break;
        case GLOBAL_MODE_PC:
            gimbal_calculate();
            chassis_calculate();
            shoot_calculate();
            //engineering_arm_calculate();
            break;
        case GLOBAL_MODE_ENGINEERING_ARM:
            gimbal_calculate();
            CHASSIS_ZERO_CURRENT();
            SHOOT_ZERO_CURRENT();
            //engineering_arm_calculate();
            break;
        default:
            CHASSIS_ZERO_CURRENT();
            GIMBAL_ZERO_CURRENT();
            SHOOT_ZERO_CURRENT();
    }

}

static THD_WORKING_AREA(mode_handle_wa, 256);

static THD_FUNCTION(mode_handle, p) {
    (void) p;
    chRegSetThreadName("mode_handle");

    while (true) {
        handle_global_mode();
        // Todo: Find better way to handle safety mode
        if (global_mode == GLOBAL_MODE_SAFETY) {
            CHASSIS_ZERO_CURRENT();
            GIMBAL_ZERO_CURRENT();
            SHOOT_ZERO_CURRENT();
            send_chassis_currents();
            send_gimbal_shoot_currents();
            send_shoot_currents();
        }
        chThdSleepMilliseconds(100);
    }
}

void mode_handle_thread_init(void) {
    chThdCreateStatic(mode_handle_wa, sizeof(mode_handle_wa), HIGHPRIO,
                      mode_handle, NULL);
}

static THD_WORKING_AREA(send_thread_wa, 256);

static THD_FUNCTION(send_thread, p) {
    (void) p;
    chRegSetThreadName("send_thread");

    while (true) {
        send_chassis_currents();
        send_gimbal_shoot_currents();
        send_shoot_currents();
        chThdSleepMilliseconds(150);
    }
}

void send_thread_init(void) {
    chThdCreateStatic(send_thread_wa, sizeof(send_thread_wa), NORMALPRIO,
                      send_thread, NULL);
}