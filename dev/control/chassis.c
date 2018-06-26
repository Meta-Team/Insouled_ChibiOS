//
// Created by 岑帜 on 2018/3/18.
//
#include "chassis.h"

#include "remote.h"
#include "mode_handle.h"

#include "modules/pid.h"

chassis_t chassis;

//pid_unit chassis_pid[4];
float pid_kp, pid_ki, pid_kd, pid_i_limit, pid_out_limit;

/**
 * Calculate rpm for each motors, crop velocities and apply pid
 */
static inline void calculate_current(void) {

    // Calculate speeds of each motors
    float rotate_ratio = ((CHASSIS_WHEELBASE + CHASSIS_WHEELTRACK) / 2.0f) / CHASSIS_RADIAN_COEF;
    float rpm_ratio = (60.0f / (CHASSIS_WHEEL_PERIMETER * CHASSIS_DECELE_RATIO));
    chassis.motor[CHASSIS_MOTOR_FR].target_rpm = (int) ((-chassis.vx + chassis.vy + chassis.w * rotate_ratio) *
                                                            rpm_ratio);
    chassis.motor[CHASSIS_MOTOR_FL].target_rpm = (int) ((chassis.vx + chassis.vy + chassis.w * rotate_ratio) *
                                                            rpm_ratio);
    chassis.motor[CHASSIS_MOTOR_BL].target_rpm = (int) ((chassis.vx - chassis.vy + chassis.w * rotate_ratio) *
                                                            rpm_ratio);
    chassis.motor[CHASSIS_MOTOR_BR].target_rpm = (int) ((-chassis.vx - chassis.vy + chassis.w * rotate_ratio) *
                                                            rpm_ratio);

    // Clip the max wheel speeds
    int max_motor_rpm = 0;
    FOR_CHASSIS_MOTOR_INDEX(i) {
        if (abs(chassis.motor[i].target_rpm) > max_motor_rpm)
            max_motor_rpm = abs(chassis.motor[i].target_rpm);
    }
    if (max_motor_rpm > CHASSIS_MOTOR_MAX_RPM) {
        float rate = (float) CHASSIS_MOTOR_MAX_RPM / max_motor_rpm;
        FOR_CHASSIS_MOTOR_INDEX(i) {
            chassis.motor[i].target_rpm *= rate;
        }
    }

    // Apply PIDs to calculate currents
    FOR_CHASSIS_MOTOR_INDEX(i) {
        chassis_motor_t *motor = &chassis.motor[i];
        motor->target_current = (int) pid_calc(&motor->pid, (float) motor->actual_rpm, (float) motor->target_rpm);
    }
}

/**
 * Handle chassis behavior in PC mode
 */
void chassis_calc_pc(void) {
    if (keyboard.press_a && !keyboard.press_d) chassis.vy = -1.0f;
    else if (keyboard.press_d && !keyboard.press_a) chassis.vy = 1.0f;
    else chassis.vy = 0.0f;


    if (keyboard.press_w && !keyboard.press_s) chassis.vx = 1.0f;
    else if (keyboard.press_s && !keyboard.press_w) chassis.vx = -1.0f;
    else chassis.vx = 0.0f;


    if (keyboard.press_q && !keyboard.press_e) chassis.w = -1.0f;
    else if (keyboard.press_e && !keyboard.press_q) chassis.w = 1.0f;
    else chassis.w = 0.0f;

    if (pc_mode == PC_MODE_NORMAL) {
        chassis.vy *= CHASSIS_PC_NORMAL_SPEED_Y;
        chassis.vx *= CHASSIS_PC_NORMAL_SPEED_X;
        chassis.w  *= CHASSIS_PC_NORMAL_W;
    } else if (pc_mode == PC_MODE_ENGI) {
        chassis.vy *= CHASSIS_PC_ENGI_SPEED_Y;
        chassis.vx *= CHASSIS_PC_ENGI_SPEED_X;
        chassis.w  *= CHASSIS_PC_ENGI_W;
    }

    calculate_current();
}

/**
 * Handle chassis behavior in remote mode
 */
void chassis_calc_remote(void) {

    chassis.vx = remote.ch1 * CHASSIS_RC_MAX_SPEED_X;
    chassis.vy = remote.ch0 * CHASSIS_RC_MAX_SPEED_Y;
    chassis.w = remote.ch2 * CHASSIS_RC_MAX_W;

    calculate_current();
}

void chassis_change_pid_based_on_pc_mode(void) {

    if (pc_mode == PC_MODE_NORMAL) {
        pid_kp = CHASSIS_PID_KP_NORMAL;
        pid_ki = CHASSIS_PID_KI_NORMAL;
        pid_kd = CHASSIS_PID_KD_NORMAL;
        pid_i_limit = CHASSIS_PID_I_LIMIT_NORMAL;
        pid_out_limit = CHASSIS_PID_OUT_LIMIT_NORMAL;
    } else if (pc_mode == PC_MODE_ENGI) {
        pid_kp = CHASSIS_PID_KP_ENGI;
        pid_ki = CHASSIS_PID_KI_ENGI;
        pid_kd = CHASSIS_PID_KD_ENGI;
        pid_i_limit = CHASSIS_PID_I_LIMIT_ENGI;
        pid_out_limit = CHASSIS_PID_OUT_LIMIT_ENGI;
    }

    FOR_CHASSIS_MOTOR_INDEX(i) {
        pid_change(&chassis.motor[i].pid, pid_kp, pid_ki, pid_kd, pid_i_limit, pid_out_limit);
    }
}

/**
 * Initialize chassis calculation
 */
void chassis_calc_init(void) {
    pid_kp = CHASSIS_PID_KP_NORMAL;
    pid_ki = CHASSIS_PID_KI_NORMAL;
    pid_kd = CHASSIS_PID_KD_NORMAL;
    pid_i_limit = CHASSIS_PID_I_LIMIT_NORMAL;
    pid_out_limit = CHASSIS_PID_OUT_LIMIT_NORMAL;
    FOR_CHASSIS_MOTOR_INDEX(i) {
        pid_init(&chassis.motor[i].pid, pid_kp, pid_ki, pid_kd, pid_i_limit, pid_out_limit);
    }
}