//
// Created by liuzikai on 30/03/2018.
//

#ifndef INSOULED_CHIBIOS_PID_H
#define INSOULED_CHIBIOS_PID_H

#include "../global.h"

struct pid_t
{
    float kp;
    float ki;
    float kd;

    float now;
    float target;
    float error[3];

    float p_out;
    float i_out;
    float d_out;

    float i_limit;
    float out_limit;

    float out;
};

void pid_init(struct pid_t *pid, float kp, float ki, float kd, float i_limit, float out_limit);
void pid_calc(struct pid_t *pid, float now, float target);

#endif //INSOULED_CHIBIOS_PID_H
