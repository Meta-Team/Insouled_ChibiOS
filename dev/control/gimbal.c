//
// Created by 岑帜 on 2018/3/30.
//

#include "gimbal.h"

struct pid_t pid_pitch;

gimbal_t gimbal;

void gimbal_calculate(void) {
    switch (global_mode) {
        case GLOBAL_MODE_SAFETY: {
            GIMBAL_ZERO_CURRENT();

            break;
        }
        case GLOBAL_MODE_PC: {
            GIMBAL_ZERO_CURRENT();

            break;
        }
        case GLOBAL_MODE_REMOTE_CHASSIS: {
            GIMBAL_ZERO_CURRENT();

            break;
        }
        case GLOBAL_MODE_REMOTE_GIMBAL: {
            gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle = remote.ch0 * GIMBAL_ANGLE_REMOTE_RATIO;
            gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle = remote.ch1 * GIMBAL_ANGLE_REMOTE_RATIO;
            gimbal.motor[GIMBAL_MOTOR_YAW].target_angle = gimbal.motor[GIMBAL_MOTOR_YAW].present_angle + gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle;
            gimbal.motor[GIMBAL_MOTOR_PIT].target_angle = gimbal.motor[GIMBAL_MOTOR_PIT].present_angle + gimbal.motor[GIMBAL_MOTOR_PIT].delta_angle;

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

            gimbal.motor[GIMBAL_MOTOR_YAW].target_current = (int16_t)(gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle * GIMBAL_YAW_CURRENT_ANGLE_RATIO);

            pid_calc(&pid_pitch, gimbal.motor[GIMBAL_MOTOR_PIT].present_angle, gimbal.motor[GIMBAL_MOTOR_PIT].target_angle);
            gimbal.motor[GIMBAL_MOTOR_PIT].target_current = (int16_t)(pid_pitch.out);

            break;
        }
    }
}

static THD_WORKING_AREA(gimbal_calc_wa, 256);

static THD_FUNCTION(gimbal_calc, p) {

    (void) p;
    chRegSetThreadName("gimbal_calc");

    while (true) {
        gimbal_calculate();
        //TODO: Modify the time interval
        chThdSleepMilliseconds(20);
    }
}

void gimbal_calc_init(void) {
    pid_init(&pid_pitch, GIMBAL_PITCH_CURRENT_ANGLE_RATIO, 0, 0, 0, GIMBAL_MOTOR_MAX_CURRENT);

    //TODO: Understand and modify the priority of the thread.
    chThdCreateStatic(gimbal_calc_wa, sizeof(gimbal_calc_wa), NORMALPRIO + 6,
                      gimbal_calc, NULL);
}