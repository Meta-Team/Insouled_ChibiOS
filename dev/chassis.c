//
// Created by 岑帜 on 2018/3/18.
//

#include "chassis.h"


void chassis_ctrl(); {
    switch (global_mode)
    {
        case GLOBAL_SAFETY:
        {
            chassis_motor_current[0] = 0;
            chassis_motor_current[1] = 0;
            chassis_motor_current[2] = 0;
            chassis_motor_current[3] = 0;
        }
        case GLOBAL_PC:
        {

        }
        case GLOBAL_REMOTE_CHASSIS:
        {

        }
        case GLOBAL_REMOTE_GIMBAL:
        {
            chassis_motor_current[0] = 0;
            chassis_motor_current[1] = 0;
            chassis_motor_current[2] = 0;
            chassis_motor_current[3] = 0;
        }
    }


}