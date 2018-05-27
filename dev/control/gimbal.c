//
// Created by 岑帜 on 2018/3/30.
//

#include <info_interaction/remote.h>
#include "gimbal.h"

struct pid_t pid_yaw;
struct pid_t pid_pitch;

gimbal_t gimbal;

static void calculate_gimbal(float yaw, float pitch) {
    chSysLock();
    if (!EQUAL_ZERO(yaw)) {
                gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle = (int) (SIGN(yaw) * GIMBAL_ANGLE_REMOTE_RATIO_YAW);
                gimbal.motor[GIMBAL_MOTOR_YAW].target_angle =
                        gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle + gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle;
            } else {
                gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle =
                        gimbal.motor[GIMBAL_MOTOR_YAW].target_angle - gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle;
            }
    if (!EQUAL_ZERO(pitch)) {
                gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle = (int) (SIGN(pitch) *
                                                                    (pitch > 0 ? GIMBAL_ANGLE_REMOTE_RATIO_PIT_UP
                                                                                    : GIMBAL_ANGLE_REMOTE_RATIO_PIT_DOWN));
                gimbal.motor[GIMBAL_MOTOR_PIT].target_angle =
                        gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle + gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle;
            } else {
                gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle =
                        gimbal.motor[GIMBAL_MOTOR_PIT].target_angle - gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle;
            }


    if (gimbal.motor[GIMBAL_MOTOR_YAW].target_angle > GIMBAL_YAW_MAX_ANGLE) {
                gimbal.motor[GIMBAL_MOTOR_YAW].target_angle = GIMBAL_YAW_MAX_ANGLE;
            }
    if (gimbal.motor[GIMBAL_MOTOR_YAW].target_angle< GIMBAL_YAW_MIN_ANGLE) {
                gimbal.motor[GIMBAL_MOTOR_YAW].target_angle = GIMBAL_YAW_MIN_ANGLE;
            }
    if (gimbal.motor[GIMBAL_MOTOR_PIT].target_angle > GIMBAL_PIT_MAX_ANGLE) {
                gimbal.motor[GIMBAL_MOTOR_PIT].target_angle = GIMBAL_PIT_MAX_ANGLE;
            }
    if (gimbal.motor[GIMBAL_MOTOR_PIT].target_angle< GIMBAL_PIT_MIN_ANGLE) {
                gimbal.motor[GIMBAL_MOTOR_PIT].target_angle = GIMBAL_PIT_MIN_ANGLE;
            }
    pid_calc(&pid_yaw, (float)gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle, (float)gimbal.motor[GIMBAL_MOTOR_YAW].target_angle);
    gimbal.motor[GIMBAL_MOTOR_YAW].target_current = (int16_t)pid_yaw.out;
    pid_calc(&pid_pitch, (float)gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle, (float)gimbal.motor[GIMBAL_MOTOR_PIT].target_angle);
    gimbal.motor[GIMBAL_MOTOR_PIT].target_current = (int16_t)pid_pitch.out;

    chSysUnlock();
}

void gimbal_calculate(void) {
    /*if (global_mode == GLOBAL_MODE_REMOTE_GIMBAL) LED_G_ON();
    else LED_G_OFF();*/

    switch (global_mode) {
        case GLOBAL_MODE_SAFETY: {
            GIMBAL_ZERO_CURRENT();

            break;
        }
        case GLOBAL_MODE_PC: {
            calculate_gimbal(mouse.x, -mouse.y);

            break;
        }
        case GLOBAL_MODE_REMOTE_CHASSIS: {
            GIMBAL_ZERO_CURRENT();

            break;
        }
        case GLOBAL_MODE_REMOTE_GIMBAL: {

            calculate_gimbal(remote.ch0, remote.ch1);

            break;
        }
    }
}

void gimbal_calc_init(void) {

    for (int i = 0; i < 2; ++i) {
        gimbal.motor[i].default_angle = -1000;
        gimbal.motor[i].actual_angle = 0;
        gimbal.motor[i].target_angle = 0;
        gimbal.motor[i].delta_angle = 0;
    }

    pid_init(&pid_yaw, 8.0, 0, 0, 0, GIMBAL_MOTOR_MAX_CURRENT);
    pid_init(&pid_pitch, 280.0, 2, 0, 1000.0, GIMBAL_MOTOR_MAX_CURRENT);
}