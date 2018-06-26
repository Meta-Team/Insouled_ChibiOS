//
// Created by 岑帜 on 2018/3/18.
//

#ifndef INSOULED_CHASSIS_H
#define INSOULED_CHASSIS_H

#include "../global.h"
#include "../modules/pid.h"

/********** Chassis Behavior Parameters **********/

/* Chassis translation speed (mm/s) */
#define CHASSIS_RC_MAX_SPEED_X  2000.0f //3300.0f
#define CHASSIS_RC_MAX_SPEED_Y  2000.0f //3300.0f

#define CHASSIS_PC_NORMAL_SPEED_X  2000.0f //3300.0f
#define CHASSIS_PC_NORMAL_SPEED_Y  2000.0f //3300.0f

#define CHASSIS_PC_ENGI_SPEED_X  800.0f
#define CHASSIS_PC_ENGI_SPEED_Y  800.0f

/* Chassis rotation speed (deg/s) */
#define CHASSIS_RC_MAX_W 200.0f //300.0f

#define CHASSIS_PC_NORMAL_W 200.0f //300.0f

#define CHASSIS_PC_ENGI_W 85.0f

/* Chassis motor speed (rad/min) */
#define CHASSIS_MOTOR_MAX_RPM 5000 //8500

/* Chassis motor current (mA) */
#define CHASSIS_MOTOR_MAX_CURRENT 5000 //20000

/* Chassis PID parameters (mA) */
#define CHASSIS_PID_KP_NORMAL 0.7f
#define CHASSIS_PID_KI_NORMAL 0.0f
#define CHASSIS_PID_KD_NORMAL 0.2f
#define CHASSIS_PID_I_LIMIT_NORMAL 0.0f
#define CHASSIS_PID_OUT_LIMIT_NORMAL 2000.0f //Current (mA)

#define CHASSIS_PID_KP_ENGI 0.5f
#define CHASSIS_PID_KI_ENGI 0.0f
#define CHASSIS_PID_KD_ENGI 0.1f
#define CHASSIS_PID_I_LIMIT_ENGI 0.0f
#define CHASSIS_PID_OUT_LIMIT_ENGI 2000.0f //Current (mA)

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

#define CHASSIS_MOTOR_COUNT 4

typedef struct {

    // Raw values storage
    uint16_t actual_angle_raw;
    int16_t actual_rpm;
    int16_t actual_current;
    uint8_t actual_temperature;


    // Normal values storage

    int target_rpm;
    int target_current;

    int actual_angle;

    pid_unit pid;

} chassis_motor_t;

typedef struct {
    // These velocities are for the whole chassis
    float vx;
    float vy;
    float w;

    chassis_motor_t motor[4];
} chassis_t;
extern chassis_t chassis;

#define CHASSIS_ZERO_CURRENT() { \
    chassis.motor[CHASSIS_MOTOR_FR].target_current = 0; \
    chassis.motor[CHASSIS_MOTOR_FL].target_current = 0; \
    chassis.motor[CHASSIS_MOTOR_BL].target_current = 0; \
    chassis.motor[CHASSIS_MOTOR_BR].target_current = 0; \
}

#define FOR_CHASSIS_MOTOR_INDEX(i) for (int (i) = 0; (i) < CHASSIS_MOTOR_COUNT; (i)++)

void chassis_calc_pc(void);
void chassis_calc_remote(void);
void chassis_change_pid_based_on_pc_mode(void);
void chassis_calc_init(void);

#endif //INSOULED_CHASSIS_H




