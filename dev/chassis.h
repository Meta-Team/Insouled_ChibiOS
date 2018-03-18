//
// Created by 岑帜 on 2018/3/18.
//

#ifndef INSOULED_CHASSIS_H
#define INSOULED_CHASSIS_H

typedef enum
{
    LEVER_UP =  1; //up
    LEVER_MD =  0; //middle
    LEVER_DW = -1; //dowm
} lever_state;

struct
{
    lever_e left_lever;
    lever_e right_lever;

    float ch0; //right horizontal
    float ch1; //right vertical
    float ch2; //left horizontal
    float ch3; //left vertical

} remote_info;
remote_info remote;

typedef enum
{
    GLOBAL_SAFETY = 0;
    GLOBAL_PC = 1;
    GLOBAL_REMOTE_CHASSIS = 2;
    GLOBAL_REMOTE_GIMBAL = 3;

} global_mode_e;
global_mode_e global_mode;

#define CHASSIS_MOTOR_FR 0u
#define CHASSIS_MOTOR_BR 1u
#define CHASSIS_MOTOR_FL 2u
#define CHASSIS_MOTOR_BL 3u

struct
{
    float vx;
    float vy;
    float w;
    float wheel_speed[4];
} chassis_s;
chassis_s chassis

unsigned int chassis_motor_current[4];

#difine

#endif //INSOULED_CHASSIS_H




