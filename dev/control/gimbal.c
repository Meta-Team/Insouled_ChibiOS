//
// Created by 岑帜 on 2018/3/30.
//

#include "gimbal.h"
#include "../global.h"
#include "pid.h"

struct pid_t pid_pitch;

void gimbal_init() {
    pid_init(&pid_pitch, GIMBAL_PITCH_CURRENT_ANGLE_RATIO, 0, 0, 0, GIMBAL_MOTOR_MAX_CURRENT);
}


void gimbal_ctrl() {
    switch (global_mode) {
        case

            GLOBAL_MODE_SAFETY: {
            gimbal.motor_current[0] = 0;
            gimbal.motor_current[1] = 0;
        }
        case

            GLOBAL_MODE_PC: {

        }
        case

            GLOBAL_MODE_REMOTE_CHASSIS: {
            gimbal.motor_current[0] = 0;
            gimbal.motor_current[1] = 0;
        }
        case

            GLOBAL_MODE_REMOTE_GIMBAL: {
            yaw.delta_angle = remote.ch0 * GIMBAL_ANGLE_REMOTE_RATIO;
            pit.delta_angle = remote.ch1 * GIMBAL_ANGLE_REMOTE_RATIO;
            yaw.target_angle = yaw.present_angle + yaw.delta_angle;
            pit.target_angle = pit.present_angle + pit.delta_angle;

            if (yaw.target_angle > YAW_MAX_ANGLE) {
                yaw.target_angle = YAW_MAX_ANGLE;
            }
            if (yaw.target_angle< YAW_MIN_ANGLE) {
                yaw.target_angle = YAW_MIN_ANGLE;
            }
            if (pit.target_angle > PIT_MAX_ANGLE) {
                pit.target_angle = PIT_MAX_ANGLE;
            }
            if (pit.target_angle< PIT_MIN_ANGLE) {
                pit.target_angle = PIT_MIN_ANGLE;
            }

            gimbal.motor_current[GIMBAL_MOTOR_YAW] = (int)(yaw.delta_angle * GIMBAL_YAW_CURRENT_ANGLE_RATIO);

            pid_calc(&pid_pitch, pit.present_angle, pit.target_angle);
            gimbal.motor_current[GIMBAL_MOTOR_PIT] = (int)(pid_pitch.out);
        }
    }
}
