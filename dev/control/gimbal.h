// Created by 岑帜 on 2018/3/30.
//

#ifndef INSOULED_GIMBAL_H
#define INSOULED_GIMBAL_H

#include "global.h"

/********** Chassis Behavior Parameters **********/

/* Angles (degree) */
#define GIMBAL_YAW_MAX_ANGLE  90
#define GIMBAL_YAW_MIN_ANGLE (-90)
#define GIMBAL_PIT_MAX_ANGLE  15
#define GIMBAL_PIT_MIN_ANGLE (-10)

#define GIMBAL_YAW_DELTA_ANGLE 30
#define GIMBAL_PIT_DELTA_ANGLE 10

#define GIMBAL_MOTOR_MAX_DELTA_ANGLE 180

/* PID Parameters */
#ifdef INFANTRY1
#define GIMBAL_YAW_PID_KP 7.0f
#define GIMBAL_YAW_PID_KI 0.0f
#define GIMBAL_YAW_PID_KD 0.0f
#define GIMBAL_YAW_PID_I_LIMIT 4000.0f
#define GIMBAL_YAW_PID_OUT_LIMIT 4000.0f //Current (mA)

#define GIMBAL_PIT_PID_KP 45.0f
#define GIMBAL_PIT_PID_KI 1.000f
#define GIMBAL_PIT_PID_KD 0.0f
#define GIMBAL_PIT_PID_I_LIMIT 4000.0f
#define GIMBAL_PIT_PID_OUT_LIMIT 4000.0f //Current (mA)
#endif

#ifdef ENGINEER
#define GIMBAL_YAW_PID_KP 7.0f
#define GIMBAL_YAW_PID_KI 0.0f
#define GIMBAL_YAW_PID_KD 0.0f
#define GIMBAL_YAW_PID_I_LIMIT 4000.0f
#define GIMBAL_YAW_PID_OUT_LIMIT 4000.0f //Current (mA)

#define GIMBAL_PIT_PID_KP 45.0f
#define GIMBAL_PIT_PID_KI 1.000f
#define GIMBAL_PIT_PID_KD 0.0f
#define GIMBAL_PIT_PID_I_LIMIT 4000.0f
#define GIMBAL_PIT_PID_OUT_LIMIT 4000.0f //Current (mA)
#endif


/********** Gimbal Structure Parameters **********/

/* "Front" Angles (orig angle) */
#ifdef INFANTRY1
#define GIMBAL_FI_YAW_ORIG 904
#define GIMBAL_FI_PIT_ORIG 7200
#endif

#ifdef ENGINEER
#define GIMBAL_FI_YAW_ORIG 0
#define GIMBAL_FI_PIT_ORIG 0
#endif

/* Motor Current (mA) */
#define GIMBAL_MOTOR_MAX_CURRENT 4000 //5000

/********** Gimbal Control Definitions **********/

// These IDs are corresponding to motor IDs
#define GIMBAL_MOTOR_YAW 0
#define GIMBAL_MOTOR_PIT 1

typedef struct {
    int16_t actual_angle_last; //last actual angle
    int16_t actual_angle_base_round; // The number of round(s) that motor has rotated related to original position
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
void gimbal_debug_print_angle(void);
#endif

#endif //INSOULED_GIMBAL_H

