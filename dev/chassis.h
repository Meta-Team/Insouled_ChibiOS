//
// Created by 岑帜 on 2018/3/18.
//

#ifndef INSOULED_CHASSIS_H
#define INSOULED_CHASSIS_H

typedef enum
{
    LEVER_UP =  1, //up
    LEVER_MD =  0, //middle
    LEVER_DW = -1, //dowm
} lever_state;

typedef struct
{
    lever_state left_lever;
    lever_state right_lever;

    float ch0; //right horizontal
    float ch1; //right vertical
    float ch2; //left horizontal
    float ch3; //left vertical

} remote_info_t;
remote_info_t remote;

typedef enum
{
    GLOBAL_MODE_SAFETY = 0,
    GLOBAL_MODE_PC = 1,
    GLOBAL_MODE_REMOTE_CHASSIS = 2,
    GLOBAL_MODE_REMOTE_GIMBAL = 3,

} global_mode_e;
global_mode_e global_mode;

#define CHASSIS_MOTOR_FR 0u
#define CHASSIS_MOTOR_BR 1u
#define CHASSIS_MOTOR_FL 2u
#define CHASSIS_MOTOR_BL 3u

typedef struct
{
    float vx;
    float vy;
    float w;
    int motor_speed[4];
    int motor_current[4];
} chassis_t;
chassis_t chassis;


/* Chassis move speed (mm/s) */
#define CHASSIS_RC_MAX_SPEED_X  2000.0f //3300.0f
#define CHASSIS_RC_MAX_SPEED_Y  2000.0f //3300.0f

/* Chassis rotation speed (deg/s) */
#define CHASSIS_RC_MAX_W 200.0f //300.0f

#define CHASSIS_MOTOE_MAX_SPEED 5000 //8500
#define CHASSIS_MOTOR_MAX_CURRENT 5000 //20000

#define CHASSIS_WHEEL_RADIUS     76 //mm
#define CHASSIS_WHEEL_PERIMETER  478 //mm

/* the deceleration ratio of chassis motor */
#define CHASSIS_DECELE_RATIO (1.0f/19.0f)

/* wheel track distance(mm) */
#define CHASSIS_WHEELTRACK 403
/* wheelbase distance(mm) */
#define CHASSIS_WHEELBASE  385

/* radian coefficient */
#define CHASSIS_RADIAN_COEF 57.3f



#endif //INSOULED_CHASSIS_H




