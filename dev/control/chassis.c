//
// Created by 岑帜 on 2018/3/18.
//

#include <info_interaction/remote.h>
#include "chassis.h"

chassis_t chassis;
struct pid_t chassis_pid;

static void calculate_current(void) {
    //Calculate speeds of each wheel
    float rotate_ratio = ((CHASSIS_WHEELBASE + CHASSIS_WHEELTRACK) / 2.0f) / CHASSIS_RADIAN_COEF;
    float rpm_ratio = (60.0f / (CHASSIS_WHEEL_PERIMETER * CHASSIS_DECELE_RATIO));
    chassis.motor_speed[0] = (int) ((-chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
    chassis.motor_speed[1] = (int) ((chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
    chassis.motor_speed[2] = (int) ((chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
    chassis.motor_speed[3] = (int) ((-chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);

    // Limit the max wheel speeds
    int max_motor_speed = 0;
    for (int i = 0; i < 4; i++) {
        if (abs(chassis.motor_speed[i]) > max_motor_speed)
            max_motor_speed = abs(chassis.motor_speed[i]);
    }

    if (max_motor_speed > CHASSIS_MOTOE_MAX_SPEED) {
        LED_R_TOGGLE();
        float rate = (float) CHASSIS_MOTOE_MAX_SPEED / max_motor_speed;
        for (int i = 0; i < 4; i++)
            chassis.motor_speed[i] *= rate;
    }

    for (int i = 0; i < 4; i++) {
        chassis.motor_current[i] = (int16_t) ((float) chassis.motor_speed[i] / CHASSIS_MOTOE_MAX_SPEED * CHASSIS_MOTOR_MAX_CURRENT);
    }
}

void chassis_calculate(void) {

    switch (global_mode) {
        case GLOBAL_MODE_SAFETY: {
            CHASSIS_ZERO_CURRENT();
            break;
        }
        case GLOBAL_MODE_PC: {

            if (keyboard.press_a ^ keyboard.press_d) {
                if(keyboard.press_a) chassis.vy = 0.3f * CHASSIS_PC_MAX_SPEED_Y;
                else if(keyboard.press_d) chassis.vy = -0.3f * CHASSIS_PC_MAX_SPEED_Y;
            } else {
                chassis.vy = 0.0f;
            }

            if (keyboard.press_w ^ keyboard.press_s) {
                if(keyboard.press_w) chassis.vx = 0.3f * CHASSIS_PC_MAX_SPEED_X;
                else if(keyboard.press_s) chassis.vx = -0.3f * CHASSIS_PC_MAX_SPEED_X;
            } else {
                chassis.vx = 0.0f;
            }

            if (keyboard.press_q ^ keyboard.press_e) {
                if (keyboard.press_q) chassis.w = 0.5f * CHASSIS_PC_MAX_W;
                else if(keyboard.press_e) chassis.w = -0.5f * CHASSIS_PC_MAX_W;
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
    pid_init(&chassis_pid, 1.50, 0.0, 0.0, 0.0, 3000.0);
}