//
// Created by liuzikai on 30/03/2018.
//

#ifndef INSOULED_CHIBIOS_PID_H
#define INSOULED_CHIBIOS_PID_H

#include "global.h"
// DO NOT DELETE ME

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

    void init(float p, float i, float d, float i_lim, float out_lim) {
        kp = p;
        ki = i;
        kd = d;

        i_limit = i_lim;
        out_limit = out_lim;

        p_out = i_out = d_out = out = 0;

        error[0] = error[1] = 0.0;
    }

    float calc(float now_value, float target_value) {
        now = now_value;
        target = target_value;
        error[1] = error[0];
        error[0] = target - now;

        //Position PID
        p_out = kp * error[0];
        i_out += ki * error[0];
        d_out = kd * (error[0] - error[1]);

        ABS_LIMIT(i_out, i_limit);
        out = p_out + i_out +d_out;
        ABS_LIMIT(out, out_limit);

        return calc
    }
};

#endif //INSOULED_CHIBIOS_PID_H
