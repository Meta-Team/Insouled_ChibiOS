//
// Created by liuzikai on 30/03/2018.
//

#ifndef INSOULED_CHIBIOS_PID_H
#define INSOULED_CHIBIOS_PID_H

#include "../global.h"

typedef struct {
    float kp;
    float ki;
    float kd;

//    float now;
//    float target;
    float error[2];

    float p_out;
    float i_out;
    float d_out;

    float i_limit;
    float out_limit;

    float out;
} pid_unit;

void pid_change(pid_unit *pid, float kp, float ki, float kd, float i_limit, float out_limit);
void pid_init(pid_unit *pid, float kp, float ki, float kd, float i_limit, float out_limit);
float pid_calc(pid_unit *pid, float now, float target);

#endif //INSOULED_CHIBIOS_PID_H