//
// Created by liuzikai on 2018/4/4.
//

#include "mode_handle.h"

global_mode_t global_mode;

global_mode_t transistion_rules[3][3] = {
        {GLOBAL_MODE_SAFETY /*Up*/, GLOBAL_MODE_SAFETY         /*Down*/, GLOBAL_MODE_SAFETY        /*Middle*/}, //Up
        {GLOBAL_MODE_SAFETY /*Up*/, GLOBAL_MODE_REMOTE_CHASSIS /*Down*/, GLOBAL_MODE_REMOTE_GIMBAL /*Middle*/}, //Down
        {GLOBAL_MODE_PC     /*Up*/, GLOBAL_MODE_PC             /*Down*/, GLOBAL_MODE_PC            /*Middle*/}  //Middle
};


void mode_handle(void) {
    if(remote.left_lever >= 1 && remote.left_lever <= 3 &&
            remote.right_lever >= 1 && remote.right_lever <=3) {
        global_mode = transistion_rules[remote.left_lever - 1][remote.right_lever - 1];
    } else {
        global_mode = GLOBAL_MODE_SAFETY;
    }


    // Quick Action
    if (global_mode == GLOBAL_MODE_SAFETY) {
        CHASSIS_ZERO_CURRENT();
        GIMBAL_ZERO_CURRENT();

        send_chassis_currents();
        send_gimbal_currents();
    }
}

void mode_handle_init(void) {
    global_mode = GLOBAL_MODE_SAFETY;
}