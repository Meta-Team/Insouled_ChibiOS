// Created by 岑帜 on 2018/3/30.
//

#ifndef INSOULED_GIMBAL_H
#define INSOULED_GIMBAL_H

#include "global.h"
#include "pid.h"

#include "remote.h"

#define YAW_MAX_ANGLE  90
#define YAW_MIN_ANGLE -90
#define PIT_MAX_ANGLE  10
#define PIT_MIN_ANGLE -10

#define GIMBAL_ANGLE_REMOTE_RATIO 10

#define GIMBAL_YAW_CURRENT_ANGLE_RATIO 1.5
#define GIMBAL_PITCH_CURRENT_ANGLE_RATIO 1.5

#define GIMBAL_MOTOR_MAX_CURRENT 2000 //5000



#define GIMBAL_MOTOR_YAW 0
#define GIMBAL_MOTOR_PIT 1

typedef struct {
    int16_t motor_current[2];
} gimbal_t;
gimbal_t gimbal;

typedef struct {
    float present_angle;
    float delta_angle;
    float target_angle;
} gimbal_motor;
extern gimbal_motor yaw;
extern gimbal_motor pit;

#define GIMBAL_ZERO_CURRENT() { \
gimbal.motor_current[0] = 0; \
gimbal.motor_current[1] = 0; \
}


void gimbal_init();
void gimbal_ctrl();

#endif //INSOULED_GIMBAL_H

