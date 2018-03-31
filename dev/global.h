//
// Created by liuzikai on 18/03/2018.
//

#ifndef INSOULED_GLOBAL_H
#define INSOULED_GLOBAL_H

typedef enum  {
    LEVER_UP = 1, //up
    LEVER_MD = 0, //middle
    LEVER_DW = -1, //down
} lever_state;

struct remote_info_t {
    lever_state left_lever;
    lever_state right_lever;

    float ch0; //right horizontal
    float ch1; //right vertical
    float ch2; //left horizontal
    float ch3; //left vertical

} remote;

enum global_mode_e {
    GLOBAL_MODE_SAFETY = 0,
    GLOBAL_MODE_PC = 1,
    GLOBAL_MODE_REMOTE_CHASSIS = 2,
    GLOBAL_MODE_REMOTE_GIMBAL = 3,

} global_mode;

#define CHASSIS_MOTOR_FR 0
#define CHASSIS_MOTOR_BR 1
#define CHASSIS_MOTOR_FL 2
#define CHASSIS_MOTOR_BL 3

struct chassis_t {
    float vx;
    float vy;
    float w;
    int motor_speed[4];
    int motor_current[4];
} chassis;
#define GIMBAL_MOTOR_YAW 0
#define GIMBAL_MOTOR_PIT 1

struct gimbal_t {
    int motor_current[2];
} gimbal;

typedef struct  {
    float present_angle;
    float delta_angle;
    float target_angle;
} gimbal_motor;
gimbal_motor yaw;
gimbal_motor pit;


#endif //INSOULED_GLOBAL_H
