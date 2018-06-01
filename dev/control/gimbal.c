//
// Created by 岑帜 on 2018/3/30.
//

#include "gimbal.h"

uint16_t gimbal_fi_orig[2];

struct pid_t pid_yaw;
struct pid_t pid_pit;

gimbal_t gimbal;

static void calculate_gimbal(float yaw, float pitch) {
    chSysLock();
    if (!EQUAL_ZERO(yaw)) {
                gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle = (int16_t) (SIGN(yaw) * GIMBAL_YAW_DELTA_ANGLE);
                gimbal.motor[GIMBAL_MOTOR_YAW].target_angle =
                        gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle + gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle;
            } else {
                gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle =
                        gimbal.motor[GIMBAL_MOTOR_YAW].target_angle - gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle;
            }
    if (!EQUAL_ZERO(pitch)) {
                gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle = (int16_t) (SIGN(pitch) * GIMBAL_PIT_DELTA_ANGLE);
                gimbal.motor[GIMBAL_MOTOR_PIT].target_angle =
                        gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle + gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle;
            } else {
                gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle =
                        gimbal.motor[GIMBAL_MOTOR_PIT].target_angle - gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle;
            }

    VAL_LIMIT(gimbal.motor[GIMBAL_MOTOR_YAW].target_angle, GIMBAL_YAW_MAX_ANGLE, GIMBAL_YAW_MIN_ANGLE);
    VAL_LIMIT(gimbal.motor[GIMBAL_MOTOR_PIT].target_angle, GIMBAL_PIT_MAX_ANGLE, GIMBAL_PIT_MIN_ANGLE);

    pid_calc(&pid_yaw, (float)gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle, (float)gimbal.motor[GIMBAL_MOTOR_YAW].target_angle);
    gimbal.motor[GIMBAL_MOTOR_YAW].target_current = (int16_t)pid_yaw.out;
    pid_calc(&pid_pit, (float)gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle, (float)gimbal.motor[GIMBAL_MOTOR_PIT].target_angle);
    gimbal.motor[GIMBAL_MOTOR_PIT].target_current = (int16_t)pid_pit.out;

    chSysUnlock();
}

void gimbal_calculate(void) {

    switch (global_mode) {
        case GLOBAL_MODE_INIT: {
            GIMBAL_ZERO_CURRENT();

            break;
        }
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

    gimbal_fi_orig[GIMBAL_MOTOR_YAW] = GIMBAL_FI_YAW_ORIG;
    gimbal_fi_orig[GIMBAL_MOTOR_PIT] = GIMBAL_FI_PIT_ORIG;

    for (int i = 0; i < 2; ++i) {
        gimbal.motor[i].actual_angle = 0;
        gimbal.motor[i].target_angle = 0;
        gimbal.motor[i].delta_angle = 0;
    }

    pid_init(&pid_yaw, GIMBAL_YAW_PID_KP, GIMBAL_YAW_PID_KI, GIMBAL_YAW_PID_KD, GIMBAL_YAW_PID_I_LIMIT, GIMBAL_YAW_PID_OUT_LIMIT);
    pid_init(&pid_pit, GIMBAL_PIT_PID_KP, GIMBAL_PIT_PID_KI, GIMBAL_PIT_PID_KD, GIMBAL_PIT_PID_I_LIMIT, GIMBAL_PIT_PID_OUT_LIMIT);
}