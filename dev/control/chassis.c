//
// Created by 岑帜 on 2018/3/18.
//

#include <info_interaction/remote.h>
#include "chassis.h"

chassis_t chassis;
struct pid_t chassis_pid[4];

static void calculate_current(void) {
    //Calculate speeds of each wheel
    float rotate_ratio = ((CHASSIS_WHEELBASE + CHASSIS_WHEELTRACK) / 2.0f) / CHASSIS_RADIAN_COEF;
    float rpm_ratio = (60.0f / (CHASSIS_WHEEL_PERIMETER * CHASSIS_DECELE_RATIO));
    chassis.motor[0].target_rpm = (int) ((-chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
    chassis.motor[1].target_rpm = (int) (( chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
    chassis.motor[2].target_rpm = (int) (( chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
    chassis.motor[3].target_rpm = (int) ((-chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);

    // Limit the max wheel speeds
    int max_motor_speed = 0;
    for (int i = 0; i < 4; i++) {
        if (abs(chassis.motor[i].target_rpm) > max_motor_speed)
            max_motor_speed = abs(chassis.motor[i].target_rpm);
    }

    if (max_motor_speed > CHASSIS_MOTOE_MAX_SPEED) {
        //LED_R_TOGGLE();
        float rate = (float) CHASSIS_MOTOE_MAX_SPEED / max_motor_speed;
        for (int i = 0; i < 4; i++)
            chassis.motor[i].target_rpm *= rate;
    }

    for (int i = 0; i < 4; i++) {
        chSysLock();
        pid_calc(&chassis_pid[i], (float)chassis.motor[i].actual_rpm, (float)chassis.motor[i].target_rpm);
        chassis.motor[i].target_current = (int16_t)chassis_pid[i].out;
        chSysUnlock();
    }
}

void chassis_calculate(void) {

    switch (global_mode) {
        case GLOBAL_MODE_INIT: {
            CHASSIS_ZERO_CURRENT();

            break;
        }
        case GLOBAL_MODE_SAFETY: {
            CHASSIS_ZERO_CURRENT();

            break;
        }
        case GLOBAL_MODE_PC: {

            if (keyboard.press_a ^ keyboard.press_d) {
                if(keyboard.press_a) chassis.vy =  CHASSIS_PC_MAX_SPEED_Y;
                else if(keyboard.press_d) chassis.vy = -CHASSIS_PC_MAX_SPEED_Y;
            } else {
                chassis.vy = 0.0f;
            }

            if (keyboard.press_w ^ keyboard.press_s) {
                if(keyboard.press_w) chassis.vx = CHASSIS_PC_MAX_SPEED_X;
                else if(keyboard.press_s) chassis.vx = -CHASSIS_PC_MAX_SPEED_X;
            } else {
                chassis.vx = 0.0f;
            }

            if (keyboard.press_q ^ keyboard.press_e) {
                if (keyboard.press_q) chassis.w = CHASSIS_PC_MAX_W;
                else if(keyboard.press_e) chassis.w = -CHASSIS_PC_MAX_W;
            } else {
                chassis.w = 0.0f;
            }

            if (keyboard.press_shift) {
                chassis.vx *= 2.0f;
                chassis.vy *= 2.0f;
            }

            calculate_current();
            break;
        }

        case GLOBAL_MODE_REMOTE_CHASSIS: {

            // Calculate car speeds
            chassis.vx = remote.ch1 * CHASSIS_RC_MAX_SPEED_X;
            chassis.vy = -remote.ch0 * CHASSIS_RC_MAX_SPEED_Y;
            chassis.w = remote.ch2 * CHASSIS_RC_MAX_W;

            calculate_current();
            break;
        }

        case GLOBAL_MODE_REMOTE_GIMBAL: {
            CHASSIS_ZERO_CURRENT();

            break;
        }

    }

}

void chassis_calc_init(void) {
    for (int i = 0; i < 4; i++) {
        pid_init(&chassis_pid[i], 1.0, 0.0, 0.0, 0.0, 2000.0);
    }
}