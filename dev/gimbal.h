//
// Created by liuzikai on 30/03/2018.
//

#ifndef INSOULED_CHIBIOS_GIMBAL_H
#define INSOULED_CHIBIOS_GIMBAL_H

#define GIMBAL_MOTOR_YAW 0
#define GIMBAL_MOTOR_PITCH 1

struct gimbal_motor_t
{
    int angle;
    int current;
} gimbal[2];

#endif //INSOULED_CHIBIOS_GIMBAL_H
