//
// Created by liuzikai on 18/03/2018.
//

#ifndef INSOULED_GLOBAL_H
#define INSOULED_GLOBAL_H

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

#define GIMBAL_MOTOR_YAW 0u
#define GIMBAL_MOTOR_PIT 1u

typedef struct
{
    int motor_current[2];
} gimbal_t;
gimbal_t gimbal

typedef struct{
    float present_angle;
    float delta_angle;
    float target_angle;
}gimbal_motor;
gimbal_motor yaw;
gimbal_motor pit;


#endif //INSOULED_GLOBAL_H
