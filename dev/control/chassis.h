//
// Created by 岑帜 on 2018/3/18.
//

#ifndef INSOULED_CHASSIS_H
#define INSOULED_CHASSIS_H

#include "global.h"
#include "remote.h"
#include "mode_handle.h"

/* Chassis move speed (mm/s) */
#define CHASSIS_RC_MAX_SPEED_X  2000.0f //3300.0f
#define CHASSIS_RC_MAX_SPEED_Y  2000.0f //3300.0f

#define CHASSIS_PC_MAX_SPEED_X  2000.0f //3300.0f
#define CHASSIS_PC_MAX_SPEED_Y  2000.0f //3300.0f

/* Chassis rotation speed (deg/s) */
#define CHASSIS_RC_MAX_W 200.0f //300.0f
#define CHASSIS_PC_MAX_W 200.0f //300.0f

#define CHASSIS_MOTOE_MAX_SPEED 5000 //8500
#define CHASSIS_MOTOR_MAX_CURRENT 5000 //20000

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



#define CHASSIS_MOTOR_FR 0
#define CHASSIS_MOTOR_FL 1
#define CHASSIS_MOTOR_BL 2
#define CHASSIS_MOTOR_BR 3

typedef struct {
    int motor_speed;
    uint16_t motor_current;
    uint16_t actual_angle;
    uint16_t actual_rpm;
    uint16_t actual_current;
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
    chassis.motor_current[0] = 0; \
    chassis.motor_current[1] = 0; \
    chassis.motor_current[2] = 0; \
    chassis.motor_current[3] = 0; \
}


void chassis_calculate(void);

void chassis_calc_init(void);


#endif //INSOULED_CHASSIS_H




