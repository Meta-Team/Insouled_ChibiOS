// Created by 岑帜 on 2018/3/30.
//

#ifndef INSOULED_GIMBAL_H
#define INSOULED_GIMBAL_H

#include "global.h"
#include "pid.h"

#include "remote.h"

/********** Chassis Behavior Parameters **********/

/* Angles (degree) */
#define GIMBAL_YAW_MAX_ANGLE  90
#define GIMBAL_YAW_MIN_ANGLE (-90)
#define GIMBAL_PIT_MAX_ANGLE  5
#define GIMBAL_PIT_MIN_ANGLE (-15)

#define GIMBAL_YAW_DELTA_ANGLE 30
#define GIMBAL_PIT_DELTA_ANGLE 2

/* PID Parameters */
#define GIMBAL_YAW_PID_KP 5.0f
#define GIMBAL_YAW_PID_KI 0.0f
#define GIMBAL_YAW_PID_KD 0.0f
#define GIMBAL_YAW_PID_I_LIMIT 0.0f
#define GIMBAL_YAW_PID_OUT_LIMIT 4000.0f //Current (mA)

#define GIMBAL_PIT_PID_KP 0.0f//300.0f
#define GIMBAL_PIT_PID_KI 0.0f//1.0f
#define GIMBAL_PIT_PID_KD 0.0f
#define GIMBAL_PIT_PID_I_LIMIT 1000.0f
#define GIMBAL_PIT_PID_OUT_LIMIT 4000.0f //Current (mA)

/********** Gimbal Structure Parameters **********/

/* "Front" Angles (orig angle) */
#ifdef INFANTRY1
#define GIMBAL_FI_YAW_ORIG 904
#define GIMBAL_FI_PIT_ORIG 7000
#endif

/* Motor Current (mA) */
#define GIMBAL_MOTOR_MAX_CURRENT 4000 //5000

/********** Gimbal Control Definitions **********/

// These IDs are corresponding to motor IDs
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

extern uint16_t gimbal_fi_orig[2];

void gimbal_calc_init(void);
void gimbal_calculate(void);

#ifdef DEBUG_GIMBAL_PID
void gimbal_debug_change_operating_pid(int index);
void gimbal_debug_print_pid_parameters(int operand);
void gimbal_debug_change_pid_parameters(int operand, float delta);
#endif

#endif //INSOULED_GIMBAL_H

