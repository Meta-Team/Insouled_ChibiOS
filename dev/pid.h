//
// Created by liuzikai on 30/03/2018.
//

#ifndef INSOULED_CHIBIOS_PID_H
#define INSOULED_CHIBIOS_PID_H

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

#endif //INSOULED_CHIBIOS_PID_H
