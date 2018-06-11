// Created by 岑帜 on 2018/3/30.
//

#ifndef INSOULED_GIMBAL_H
#define INSOULED_GIMBAL_H

#include "global.h"

/********** Chassis Behavior Parameters **********/

/* Angles (degree) */

#ifdef INFANTRY1
#define GIMBAL_YAW_MAX_ANGLE  90
#define GIMBAL_YAW_MIN_ANGLE (-90)
#define GIMBAL_ROLL_MAX_ANGLE  15
#define GIMBAL_ROLL_MIN_ANGLE (-10)

#define GIMBAL_YAW_DELTA_ANGLE_NORMAL 30
#define GIMBAL_ROLL_DELTA_ANGLE 10
#endif

#ifdef ENGINEER
#define GIMBAL_YAW_MAX_ANGLE  90
#define GIMBAL_YAW_MIN_ANGLE (-90)
#define GIMBAL_ROLL_MAX_ANGLE  720
#define GIMBAL_ROLL_MIN_ANGLE (-720)

#define GIMBAL_YAW_DELTA_ANGLE_NORMAL 20
#define GIMBAL_YAW_DELTA_ANGLE_ENGI 10

#define GIMBAL_ROLL_DELTA_ANGLE 6
#endif

#define GIMBAL_MOTOR_MAX_DELTA_ANGLE 180

/* PID Parameters */
#ifdef INFANTRY1
#define GIMBAL_YAW_PID_KP_EMPTY 7.0f
#define GIMBAL_YAW_PID_KI_EMPTY 0.0f
#define GIMBAL_YAW_PID_KD_EMPTY 0.0f
#define GIMBAL_YAW_PID_I_LIMIT_EMPTY 4000.0f
#define GIMBAL_YAW_PID_OUT_LIMIT_EMPTY 4000.0f //Current (mA)

#define GIMBAL_ROLL_PID_KP_EMPTY 45.0f
#define GIMBAL_ROLL_PID_KI_EMPTY 1.000f
#define GIMBAL_ROLL_PID_KD_EMPTY 0.0f
#define GIMBAL_ROLL_PID_I_LIMIT_EMPTY 4000.0f
#define GIMBAL_ROLL_PID_OUT_LIMIT_EMPTY 4000.0f //Current (mA)
#endif

#ifdef ENGINEER
#define GIMBAL_YAW_PID_KP_EMPTY 10.0f
#define GIMBAL_YAW_PID_KI_EMPTY 0.5f
#define GIMBAL_YAW_PID_KD_EMPTY 3.0f
#define GIMBAL_YAW_PID_I_LIMIT_EMPTY 1000.0f
#define GIMBAL_YAW_PID_OUT_LIMIT_EMPTY 3000.0f //Current (mA)

#define GIMBAL_YAW_PID_KP_PICKED 12.0f
#define GIMBAL_YAW_PID_KI_PICKED 0.3f
#define GIMBAL_YAW_PID_KD_PICKED 0.8f
#define GIMBAL_YAW_PID_I_LIMIT_PICKED 1000.0f
#define GIMBAL_YAW_PID_OUT_LIMIT_PICKED 3000.0f //Current (mA)

#define GIMBAL_ROLL_PID_KP_EMPTY 12.0f
#define GIMBAL_ROLL_PID_KI_EMPTY 1.0f
#define GIMBAL_ROLL_PID_KD_EMPTY 0.0f
#define GIMBAL_ROLL_PID_I_LIMIT_EMPTY 400.0f
#define GIMBAL_ROLL_PID_OUT_LIMIT_EMPTY 600.0f //Current (mA)

#define GIMBAL_ROLL_PID_KP_PICKED 25.0f
#define GIMBAL_ROLL_PID_KI_PICKED 0.0f
#define GIMBAL_ROLL_PID_KD_PICKED 0.0f
#define GIMBAL_ROLL_PID_I_LIMIT_PICKED 1200.0f
#define GIMBAL_ROLL_PID_OUT_LIMIT_PICKED 2000.0f //Current (mA)
#endif


/********** Gimbal Structure Parameters **********/

/* "Front" Angles (orig angle) */
#ifdef INFANTRY1
#define GIMBAL_FI_YAW_ORIG 904
#define GIMBAL_FI_ROLL_ORIG 7200
#endif

#ifdef ENGINEER
#define GIMBAL_FI_YAW_ORIG 3800
#define GIMBAL_FI_ROLL_ORIG 1590
#endif

/* Motor Current (mA) */
#define GIMBAL_MOTOR_MAX_CURRENT 4000 //5000

/********** Gimbal Control Definitions **********/

// These IDs are corresponding to motor IDs
#define GIMBAL_MOTOR_YAW 0
#define GIMBAL_MOTOR_ROLL 1

typedef struct {
    uint16_t actual_angle_orig;
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
    gimbal.motor[GIMBAL_MOTOR_ROLL].target_current = 0; \
}

extern uint16_t gimbal_fi_orig[2];

void gimbal_calc_init(void);
void gimbal_init_pid_based_on_me_arm_mode(void);
void gimbal_calculate_pc(void);
void gimbal_calculate_remote(void);
void gimbal_calculate_hold(void);

#endif //INSOULED_GIMBAL_H

