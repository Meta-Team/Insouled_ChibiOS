//
// Created by liuzikai on 2018/6/1.
//

#include "main_control_loop.h"

void main_control_loop(void) {

    handle_global_mode();

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
            break;
        default:
            CHASSIS_ZERO_CURRENT();
            GIMBAL_ZERO_CURRENT();
            SHOOT_ZERO_CURRENT();
    }

    send_chassis_currents();
    send_gimbal_currents();
    send_shoot_currents();
}