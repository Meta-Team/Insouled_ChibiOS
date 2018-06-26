//
// Created by liuzikai on 2018/4/4.
//

#include "mode_handle.h"

#include "remote.h"

#include "component_handle/chassis.h"

global_mode_t global_mode;

global_mode_t transistion_rules[3][3] = {
        {GLOBAL_MODE_SAFETY /*Up*/, GLOBAL_MODE_SAFETY         /*Down*/, GLOBAL_MODE_SAFETY        /*Middle*/}, //Up
        {GLOBAL_MODE_REMOTE_ME_ARM /*Up*/, GLOBAL_MODE_REMOTE_CHASSIS /*Down*/, GLOBAL_MODE_REMOTE_GIMBAL /*Middle*/}, //Down
        {GLOBAL_MODE_PC     /*Up*/, GLOBAL_MODE_PC             /*Down*/, GLOBAL_MODE_PC            /*Middle*/}  //Middle
};


pc_mode_t pc_mode;

pc_mode_t new_pc_mode;
void handle_modes(void) {

    if(remote.left_lever >= 1 && remote.left_lever <= 3 &&
            remote.right_lever >= 1 && remote.right_lever <=3) {
        global_mode = transistion_rules[remote.left_lever - 1][remote.right_lever - 1];
    } else {
        global_mode = GLOBAL_MODE_SAFETY;
    }

    if (keyboard.press_z && !keyboard.press_x) {
        new_pc_mode = PC_MODE_NORMAL;
    } else if (!keyboard.press_z && keyboard.press_x) {
        new_pc_mode = PC_MODE_ENGI;
    }

    if (pc_mode != new_pc_mode) {
        pc_mode = new_pc_mode;
        chassis_change_pid_based_on_pc_mode();
    }
}

void mode_handle_init(void) {
    global_mode = GLOBAL_MODE_SAFETY;
    pc_mode = PC_MODE_NORMAL;
}