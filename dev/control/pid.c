//
// Created by liuzikai on 30/03/2018.
//

#include "pid.h"


void pid_init(struct pid_t *pid, float kp, float ki, float kd, float i_limit, float out_limit) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->i_limit = i_limit;
    pid->out_limit = out_limit;

    pid->p_out = pid->i_out = pid->d_out = pid->out = 0;

    pid->error[0] = pid->error[1] = 0.0;
}

void pid_calc(struct pid_t *pid, float now, float target) {
    pid->now = now;
    pid->target = target;
    pid->error[1] = pid->error[0];
    pid->error[0] = target - now;

    //Position PID
    pid->p_out = pid->kp * pid->error[0];
    pid->i_out += pid->ki * pid->error[0];
    pid->d_out = pid->kd * (pid->error[0] - pid->error[1]);

    ABS_LIMIT(pid->i_out, pid->i_limit);
    pid->out = pid->p_out + pid->i_out +pid->d_out;
    ABS_LIMIT(pid->out, pid->out_limit);
}