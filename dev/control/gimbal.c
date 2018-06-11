//
// Created by 岑帜 on 2018/3/30.
//

#include <info_interaction/remote.h>
#include "gimbal.h"

#include "pid.h"

#include "remote.h"

#include "me_arm.h"

uint16_t gimbal_fi_orig[2];

struct pid_t pid_yaw;
struct pid_t pid_pit;

gimbal_t gimbal;

static inline void calculate_yaw(float yaw, float delta_angle) {
    if (!EQUAL_ZERO(yaw)) {
        gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle = (int16_t) (SIGN(yaw) * delta_angle);
        gimbal.motor[GIMBAL_MOTOR_YAW].target_angle =
                gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle + gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle;
    } else {
        gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle =
                gimbal.motor[GIMBAL_MOTOR_YAW].target_angle - gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle;
    }

    VAL_LIMIT(gimbal.motor[GIMBAL_MOTOR_YAW].target_angle, GIMBAL_YAW_MAX_ANGLE, GIMBAL_YAW_MIN_ANGLE);

    pid_calc(&pid_yaw, (float) gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle,
             (float) gimbal.motor[GIMBAL_MOTOR_YAW].target_angle);
    gimbal.motor[GIMBAL_MOTOR_YAW].target_current = (int16_t) pid_yaw.out;
}

static inline void calculate_roll(float roll) {

    if (me_arm.mode == ME_ARM_MODE_PICKED) {
        if (!EQUAL_ZERO(roll)) {
            gimbal.motor[GIMBAL_MOTOR_ROLL].delta_angle = (int16_t) (SIGN(roll) * GIMBAL_ROLL_DELTA_ANGLE);
            gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle =
                    gimbal.motor[GIMBAL_MOTOR_ROLL].actual_angle + gimbal.motor[GIMBAL_MOTOR_ROLL].delta_angle;
        } else {
            gimbal.motor[GIMBAL_MOTOR_ROLL].delta_angle =
                    gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle - gimbal.motor[GIMBAL_MOTOR_ROLL].actual_angle;
        }
    } else {
        gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle = 0;
    }

    VAL_LIMIT(gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle, GIMBAL_ROLL_MAX_ANGLE, GIMBAL_ROLL_MIN_ANGLE);

    pid_calc(&pid_pit, (float) gimbal.motor[GIMBAL_MOTOR_ROLL].actual_angle,
             (float) gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle);
    gimbal.motor[GIMBAL_MOTOR_ROLL].target_current = (int16_t) pid_pit.out;
}

void gimbal_calculate_pc(void) {
    if (pc_mode == PC_MODE_NORMAL) {
        calculate_yaw(mouse.x, GIMBAL_YAW_DELTA_ANGLE_NORMAL);
    } else if (pc_mode == PC_MODE_ENGI) {
        calculate_yaw(mouse.x, GIMBAL_YAW_DELTA_ANGLE_ENGI);
    }

    if (me_arm.mode == ME_ARM_MODE_PICKED) {
        if (keyboard.press_g && !keyboard.press_f) {
            calculate_roll(-1.0f);
        } else if (keyboard.press_f && !keyboard.press_g) {
            calculate_roll(+1.0f);
        }

    } else {
        //TODO: Find a better way to organize functions
        calculate_roll(0.0f/*useless parameter*/);
    }

}

void gimbal_calculate_remote(void) {
    calculate_yaw(remote.ch0, GIMBAL_YAW_DELTA_ANGLE_NORMAL);
    calculate_roll(remote.ch1);
}

void gimbal_calculate_hold(void) {
    // TODO: to be be compatible
    calculate_yaw(0.0f, GIMBAL_YAW_DELTA_ANGLE_NORMAL);
    calculate_roll(0.0f);
}


void gimbal_init_pid_based_on_me_arm_mode(void) {

    if (me_arm.mode == ME_ARM_MODE_PICKED) {
        pid_init(&pid_yaw, GIMBAL_YAW_PID_KP_PICKED, GIMBAL_YAW_PID_KI_PICKED, GIMBAL_YAW_PID_KD_PICKED,
                 GIMBAL_YAW_PID_I_LIMIT_PICKED, GIMBAL_YAW_PID_OUT_LIMIT_PICKED);
        pid_init(&pid_pit, GIMBAL_ROLL_PID_KP_PICKED, GIMBAL_ROLL_PID_KI_PICKED, GIMBAL_ROLL_PID_KD_PICKED,
                 GIMBAL_ROLL_PID_I_LIMIT_PICKED, GIMBAL_ROLL_PID_OUT_LIMIT_PICKED);
    } else {
        pid_init(&pid_yaw, GIMBAL_YAW_PID_KP_EMPTY, GIMBAL_YAW_PID_KI_EMPTY, GIMBAL_YAW_PID_KD_EMPTY,
                 GIMBAL_YAW_PID_I_LIMIT_EMPTY, GIMBAL_YAW_PID_OUT_LIMIT_EMPTY);
        pid_init(&pid_pit, GIMBAL_ROLL_PID_KP_EMPTY, GIMBAL_ROLL_PID_KI_EMPTY, GIMBAL_ROLL_PID_KD_EMPTY,
                 GIMBAL_ROLL_PID_I_LIMIT_EMPTY, GIMBAL_ROLL_PID_OUT_LIMIT_EMPTY);
    }

}

void gimbal_calc_init(void) {

    gimbal_fi_orig[GIMBAL_MOTOR_YAW] = GIMBAL_FI_YAW_ORIG;
    gimbal_fi_orig[GIMBAL_MOTOR_ROLL] = GIMBAL_FI_ROLL_ORIG;

    for (int i = 0; i < 2; ++i) {
        gimbal.motor[i].actual_angle_base_round = 0;
        gimbal.motor[i].actual_angle_last = gimbal.motor[i].actual_angle = 0;
        gimbal.motor[i].target_angle = 0;
        gimbal.motor[i].delta_angle = 0;
    }

    gimbal_init_pid_based_on_me_arm_mode();
}