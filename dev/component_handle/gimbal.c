//
// Created by 岑帜 on 2018/3/30.
//

#include <info_interaction/remote.h>
#include "gimbal.h"

#include "modules/pid.h"

#include "remote.h"

#include "me_arm.h"

//uint16_t gimbal_fi_orig[2];

//struct pid_t pid_yaw;
//struct pid_t pid_pit;

gimbal_t gimbal;

/**
 * Convey raw angles to our coordinate
 */
static inline void pre_process_gimbal_feedback(void) {

    FOR_GIMBAL_MOTOR_INDEX(motor_id) {

        gimbal_motor_t *motor = &gimbal.motor[motor_id];

        motor->actual_angle = ((int) motor->actual_angle_raw - (int) motor->fi_raw) + 360 * motor->round_count;
        if (motor->fi_raw > 4096 && motor->actual_angle_raw < motor->fi_raw - 4096) {
            // Case A, red range
            motor->actual_angle += 360;
        } else if (motor->fi_raw < 4097 && motor->actual_angle_raw > motor->fi_raw + 4096) {
            // Case B, red range
            motor->actual_angle -= 360;
        }

//        float angle = ((float)feedback_angle_orig - (float)gimbal_fi_orig[motor_id]) * 360.0f / 8192.0f;
//
//        if (gimbal_fi_orig[motor_id] > 4096 && feedback_angle_orig < gimbal_fi_orig[motor_id] - 4096) //Case I, Red Range
//            angle += 360.0f;
//        if (gimbal_fi_orig[motor_id] < 4096 && feedback_angle_orig > gimbal_fi_orig[motor_id] + 4096) //Case II, Red Range
//            angle -= 360.0f;
//
//        int16_t new_angle = (int16_t) (-1.0f * angle + 360.0f * gimbal.motor[motor_id].actual_angle_base_round);
//        if (new_angle - gimbal.motor[motor_id].actual_angle_last > GIMBAL_MOTOR_MAX_DELTA_ANGLE) {
//            // From - to + across 180
//            gimbal.motor[motor_id].actual_angle_base_round--;
//            new_angle -= 360;
//        }
//        if (new_angle - gimbal.motor[motor_id].actual_angle_last < -GIMBAL_MOTOR_MAX_DELTA_ANGLE) {
//            // From + to - across 180
//            gimbal.motor[motor_id].actual_angle_base_round++;
//            new_angle += 360;
//        }
//        gimbal.motor[motor_id].actual_angle_last = gimbal.motor[motor_id].actual_angle;
//        gimbal.motor[motor_id].actual_angle = new_angle;
    }
}

/**
 * Calculate target angle and current of specific motor
 * @param motor_id
 * @param delta_angle
 */
static inline void calculate_motor(int motor_id, int delta_angle) {
    gimbal_motor_t *motor = &gimbal.motor[motor_id];
    if (delta_angle != 0) {
        // If there is instruction to move gimbal, change target_angle
        motor->target_angle = motor->actual_angle + delta_angle;
    }
    VAL_LIMIT(motor->target_angle, GIMBAL_YAW_MAX_ANGLE, GIMBAL_YAW_MIN_ANGLE);
    motor->target_current = (int) pid_calc(&motor->pid, (float) motor->actual_angle, (float) motor->target_angle);
}

//static inline void calculate_yaw(float yaw, float delta_angle) {
//    if (!EQUAL_ZERO(yaw)) {
//        gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle = (int16_t) (SIGN(yaw) * delta_angle);
//        gimbal.motor[GIMBAL_MOTOR_YAW].target_angle =
//                gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle + gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle;
//    } else {
//        gimbal.motor[GIMBAL_MOTOR_YAW].delta_angle =
//                gimbal.motor[GIMBAL_MOTOR_YAW].target_angle - gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle;
//    }
//
//    VAL_LIMIT(gimbal.motor[GIMBAL_MOTOR_YAW].target_angle, GIMBAL_YAW_MAX_ANGLE, GIMBAL_YAW_MIN_ANGLE);
//
//    pid_calc(&pid_yaw, (float) gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle,
//             (float) gimbal.motor[GIMBAL_MOTOR_YAW].target_angle);
//    gimbal.motor[GIMBAL_MOTOR_YAW].target_current = (int16_t) pid_yaw.out;
//}

//static inline void calculate_roll(int delta_angle) {
//    gimbal_motor_t *motor = &gimbal.motor[GIMBAL_MOTOR_ROLL];
//    if (!EQUAL_ZERO(roll)) {
//        gimbal.motor[GIMBAL_MOTOR_ROLL].delta_angle = (int16_t) (SIGN(roll) * GIMBAL_ROLL_DELTA_ANGLE);
//        gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle =
//                gimbal.motor[GIMBAL_MOTOR_ROLL].actual_angle + gimbal.motor[GIMBAL_MOTOR_ROLL].delta_angle;
//    } else {
//        gimbal.motor[GIMBAL_MOTOR_ROLL].delta_angle =
//                gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle - gimbal.motor[GIMBAL_MOTOR_ROLL].actual_angle;
//    }
//
//    VAL_LIMIT(gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle, GIMBAL_ROLL_MAX_ANGLE, GIMBAL_ROLL_MIN_ANGLE);
//
//    pid_calc(&pid_pit, (float) gimbal.motor[GIMBAL_MOTOR_ROLL].actual_angle,
//             (float) gimbal.motor[GIMBAL_MOTOR_ROLL].target_angle);
//    gimbal.motor[GIMBAL_MOTOR_ROLL].target_current = (int16_t) pid_pit.out;
//}

/**
 * Control gimbal behavior in pc mode
 */
void gimbal_calc_pc(void) {
    pre_process_gimbal_feedback();
    if (pc_mode == PC_MODE_NORMAL) {
        calculate_motor(GIMBAL_MOTOR_YAW, (int) mouse.x * GIMBAL_YAW_DELTA_ANGLE_NORMAL);
    } else if (pc_mode == PC_MODE_ENGI) {
        // Force gimbal to point straight forward in ENGI mode
        calculate_motor(GIMBAL_MOTOR_YAW, -gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle);
    }

    if (me_arm.mode == ME_ARM_MODE_PICKED) {
        // When in ME_ARM_MODE_PICKED, allow rotation by keyboard
        if (keyboard.press_g && !keyboard.press_f) {
            calculate_motor(GIMBAL_MOTOR_ROLL, -GIMBAL_ROLL_DELTA_ANGLE);
        } else if (keyboard.press_f && !keyboard.press_g) {
            calculate_motor(GIMBAL_MOTOR_ROLL, +GIMBAL_ROLL_DELTA_ANGLE);
        }

    } else {
        // If not, force roll motor to be 0 angle
        calculate_motor(GIMBAL_MOTOR_ROLL, -gimbal.motor[GIMBAL_MOTOR_ROLL].actual_angle);
    }

}

/**
 * Control gimbal behavior in remote mode
 */
void gimbal_calc_remote(void) {
    pre_process_gimbal_feedback();
    calculate_motor(GIMBAL_MOTOR_YAW, (int) (remote.ch0 * GIMBAL_YAW_DELTA_ANGLE_NORMAL));
    calculate_motor(GIMBAL_MOTOR_ROLL, (int) (remote.ch1 * GIMBAL_ROLL_DELTA_ANGLE));
}

/**
 * Control gimbal to hold at current position
 */
void gimbal_calc_hold(void) {
    pre_process_gimbal_feedback();
    calculate_motor(GIMBAL_MOTOR_YAW, 0);
    calculate_motor(GIMBAL_MOTOR_ROLL, 0);
}


void gimbal_change_pid_based_on_me_arm_mode(void) {

    if (me_arm.mode == ME_ARM_MODE_PICKED) {
        pid_change(&gimbal.motor[GIMBAL_MOTOR_YAW].pid, GIMBAL_YAW_PID_KP_PICKED, GIMBAL_YAW_PID_KI_PICKED, GIMBAL_YAW_PID_KD_PICKED, GIMBAL_YAW_PID_I_LIMIT_PICKED, GIMBAL_YAW_PID_OUT_LIMIT_PICKED);
        pid_change(&gimbal.motor[GIMBAL_MOTOR_ROLL].pid, GIMBAL_ROLL_PID_KP_PICKED, GIMBAL_ROLL_PID_KI_PICKED, GIMBAL_ROLL_PID_KD_PICKED, GIMBAL_ROLL_PID_I_LIMIT_PICKED, GIMBAL_ROLL_PID_OUT_LIMIT_PICKED);
    } else {
        pid_change(&gimbal.motor[GIMBAL_MOTOR_YAW].pid, GIMBAL_YAW_PID_KP_EMPTY, GIMBAL_YAW_PID_KI_EMPTY, GIMBAL_YAW_PID_KD_EMPTY, GIMBAL_YAW_PID_I_LIMIT_EMPTY, GIMBAL_YAW_PID_OUT_LIMIT_EMPTY);
        pid_change(&gimbal.motor[GIMBAL_MOTOR_ROLL].pid, GIMBAL_ROLL_PID_KP_EMPTY, GIMBAL_ROLL_PID_KI_EMPTY, GIMBAL_ROLL_PID_KD_EMPTY, GIMBAL_ROLL_PID_I_LIMIT_EMPTY, GIMBAL_ROLL_PID_OUT_LIMIT_EMPTY);
    }

}

/**
 * Initialize gimbal calculation tasks
 */
void gimbal_calc_init(void) {

    gimbal.motor[GIMBAL_MOTOR_YAW].fi_raw = GIMBAL_FI_YAW_ORIG;
    gimbal.motor[GIMBAL_MOTOR_ROLL].fi_raw = GIMBAL_FI_ROLL_ORIG;

    FOR_GIMBAL_MOTOR_INDEX(motor_id) {
        gimbal_motor_t *motor = &gimbal.motor[motor_id];

        motor->round_count = 0;
        motor->target_angle = 0;
        motor->target_current = 0;
    }

    gimbal_change_pid_based_on_me_arm_mode();

//    gimbal_fi_orig[GIMBAL_MOTOR_YAW] = GIMBAL_FI_YAW_ORIG;
//    gimbal_fi_orig[GIMBAL_MOTOR_ROLL] = GIMBAL_FI_ROLL_ORIG;
//
//    for (int i = 0; i < 2; ++i) {
//        gimbal.motor[i].actual_angle_base_round = 0;
//        gimbal.motor[i].actual_angle_last = gimbal.motor[i].actual_angle = 0;
//        gimbal.motor[i].target_angle = 0;
//        gimbal.motor[i].delta_angle = 0;
//    }
//
//    gimbal_init_pid_based_on_me_arm_mode();
}

/**
 * This function is used to initialize last_acutal_angle_raw during program initial period
 */
 // TODO: Apply this function to GLOBAL_INIT_MODE
void gimbal_calc_load(void) {
    FOR_GIMBAL_MOTOR_INDEX(motor_id) {
        gimbal.motor[motor_id].last_actual_angle_raw = gimbal.motor[motor_id].actual_angle_raw;
    }
}