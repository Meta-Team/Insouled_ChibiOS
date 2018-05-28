// Created by 岑帜 on 2018/3/30.
//

#ifndef INSOULED_GIMBAL_H
#define INSOULED_GIMBAL_H

#include "global.h"
#include "pid.h"

#include "remote.h"

#define EQUAL_ZERO(n) (n < 0.001f && n > -0.001f)

#define GIMBAL_YAW_MAX_ANGLE  90
#define GIMBAL_YAW_MIN_ANGLE (-90)
#define GIMBAL_PIT_MAX_ANGLE  5
#define GIMBAL_PIT_MIN_ANGLE (-15)

#define GIMBAL_ANGLE_REMOTE_RATIO_YAW 30
#define GIMBAL_ANGLE_REMOTE_RATIO_PIT_UP 2
#define GIMBAL_ANGLE_REMOTE_RATIO_PIT_DOWN 2

#define GIMBAL_YAW_CURRENT_ANGLE_RATIO 10
#define GIMBAL_PITCH_CURRENT_ANGLE_RATIO 1.5

#define GIMBAL_MOTOR_MAX_CURRENT 4000 //5000

#define GIMBAL_MOTOR_YAW 0
#define GIMBAL_MOTOR_PIT 1

typedef struct {
    uint16_t default_angle_orig;
    int16_t actual_angle;
    int16_t delta_angle;
    int16_t target_angle;
    int16_t target_current;
} gimbal_motor;

typedef struct {
    gimbal_motor motor[2];
} gimbal_t;
extern gimbal_t gimbal;


#define GIMBAL_ZERO_CURRENT() { \
    gimbal.motor[GIMBAL_MOTOR_YAW].target_current = 0; \
    gimbal.motor[GIMBAL_MOTOR_PIT].target_current = 0; \
}


void gimbal_calc_init(void);
void gimbal_calculate(void);

#endif //INSOULED_GIMBAL_H

