//
// Created by 岑帜 on 2018/3/30.
//

#include "gimbal.h"

#include "pid.h"

#include "remote.h"

uint16_t gimbal_fi_orig[2];

struct pid_t pid_yaw;
struct pid_t pid_pit;

gimbal_t gimbal;

#ifdef DEBUG_GIMBAL_PID

struct pid_t* debug_pid;

void gimbal_debug_change_operating_pid(int index) {
    if (index == 0) {
        debug_pid = &pid_yaw;
    } else (index == 1) {
        debug_pid = &pid_pit;
    }
}

void gimbal_debug_print_pid_parameters(int operand) {
    if (debug_pid == &pid_yaw) print("[GIMBAL_PID_YAW]"); else print("[GIMBAL_PID_PIT]");
    if (operand == 0) print("[kp]"); else print("kp");
    print(" = %f, ", debug_pid->kp);
    if (operand == 1) print("[ki]"); else print("ki");
    print(" = %f, ", debug_pid->ki);
    if (operand == 2) print("[kd]"); else print("kd");
    print(" = %f, ", debug_pid->kd);
    if (operand == 3) print("[i_limit]"); else print("i_limit");
    print(" = %f, ", debug_pid->i_limit);
    if (operand == 4) print("[out_limit]"); else print("out_limit");
    print(" = %f.\n", debug_pid->out_limit);
}

void gimbal_debug_change_pid_parameters(int operand, float delta) {
    if (operand == 0) debug_pid->kp += delta;
    if (operand == 1) debug_pid->ki += delta;
    if (operand == 2) debug_pid->kd += delta;
    if (operand == 3) debug_pid->i_limit += delta;
    if (operand == 4) debug_pid->out_limit += delta;
}

void gimbal_debug_print_angle(void) {
    if (debug_pid == &pid_yaw) print("[GIMBAL_YAW_ANGLE] actual = %d, target = %d\n", (int)gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle, (int)gimbal.motor[GIMBAL_MOTOR_YAW].target_angle);
    else print("[GIMBAL_PIT_ANGLE] actual = %d, target = %d\n", (int)gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle, (int)gimbal.motor[GIMBAL_MOTOR_PIT].target_angle);
}

#endif

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
        gimbal.motor[i].actual_angle_base_round = 0;
        gimbal.motor[i].actual_angle_last = gimbal.motor[i].actual_angle = 0;
        gimbal.motor[i].target_angle = 0;
        gimbal.motor[i].delta_angle = 0;
    }

    pid_init(&pid_yaw, GIMBAL_YAW_PID_KP, GIMBAL_YAW_PID_KI, GIMBAL_YAW_PID_KD, GIMBAL_YAW_PID_I_LIMIT, GIMBAL_YAW_PID_OUT_LIMIT);
    pid_init(&pid_pit, GIMBAL_PIT_PID_KP, GIMBAL_PIT_PID_KI, GIMBAL_PIT_PID_KD, GIMBAL_PIT_PID_I_LIMIT, GIMBAL_PIT_PID_OUT_LIMIT);

}