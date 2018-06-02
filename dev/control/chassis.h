//
// Created by 岑帜 on 2018/3/18.
//

#ifndef INSOULED_CHASSIS_H
#define INSOULED_CHASSIS_H

#include "global.h"

/********** Chassis Behavior Parameters **********/

/* Chassis translation speed (mm/s) */
#define CHASSIS_RC_MAX_SPEED_X  2000.0f //3300.0f
#define CHASSIS_RC_MAX_SPEED_Y  2000.0f //3300.0f

#define CHASSIS_PC_BASE_SPEED_X  2000.0f //3300.0f
#define CHASSIS_PC_BASE_SPEED_Y  2000.0f //3300.0f

/* Chassis rotation speed (deg/s) */
#define CHASSIS_RC_MAX_W 200.0f //300.0f

#define CHASSIS_PC_BASE_W 200.0f //300.0f

/* Chassis motor speed (rad/min) */
#define CHASSIS_MOTOR_MAX_RPM 5000 //8500

/* Chassis motor current (mA) */
#define CHASSIS_MOTOR_MAX_CURRENT 5000 //20000

/* Chassis Speed Control Ratios */
#define CHASSIS_SPEED_RATIO_SHIFT 2.0f
#define CHASSIS_SPEED_RATIO_CTRL 0.5f

/* Chassis PID parameters (mA) */
#define CHASSIS_PID_KP 1.0f
#define CHASSIS_PID_KI 0.0f
#define CHASSIS_PID_KD 0.0f
#define CHASSIS_PID_I_LIMIT 0.0f
#define CHASSIS_PID_OUT_LIMIT 2000.0f //Current (mA)

/********** Chassis Structure Parameters **********/

#define CHASSIS_WHEEL_RADIUS     76 //mm
#define CHASSIS_WHEEL_PERIMETER  478 //mm

/* the deceleration ratio of chassis motor */
#define CHASSIS_DECELE_RATIO (1.0f/19.0f)

/* wheel track distance(mm) */
#define CHASSIS_WHEELTRACK 358
/* wheelbase distance(mm) */
#define CHASSIS_WHEELBASE  358

/* radian coefficient */
#define CHASSIS_RADIAN_COEF 57.3f


/********** Chassis Control Definitions **********/

// These IDs are corresponding to motor IDs
#define CHASSIS_MOTOR_FR 0
#define CHASSIS_MOTOR_FL 1
#define CHASSIS_MOTOR_BL 2
#define CHASSIS_MOTOR_BR 3

typedef struct {
    int16_t target_rpm;
    int16_t target_current;
    uint16_t actual_angle;
    int16_t actual_rpm;
    int16_t actual_current;
    uint8_t actual_temperature;
} motor_t;

typedef struct {
    float vx;
    float vy;
    float w;
    motor_t motor[4];
} chassis_t;
extern chassis_t chassis;

#define CHASSIS_ZERO_CURRENT() { \
    chassis.motor[CHASSIS_MOTOR_FR].target_current = 0; \
    chassis.motor[CHASSIS_MOTOR_FL].target_current = 0; \
    chassis.motor[CHASSIS_MOTOR_BL].target_current = 0; \
    chassis.motor[CHASSIS_MOTOR_BR].target_current = 0; \
}

void chassis_calculate(void);

void chassis_calc_init(void);

#ifdef DEBUG_CHASSIS_PID
void chassis_debug_print_pid_parameters(int operand);
void chassis_debug_change_pid_parameters(int operand, float delta);
#endif

#endif //INSOULED_CHASSIS_H




