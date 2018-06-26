//
// Created by liuzikai on 30/03/2018.
//

#include "pid.h"

/**
 * Change parameters of a pid_unit without cleaning current state
 * @param pid
 * @param kp
 * @param ki
 * @param kd
 * @param i_limit
 * @param out_limit
 */
void pid_change(pid_unit *pid, float kp, float ki, float kd, float i_limit, float out_limit) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->i_limit = i_limit;
    pid->out_limit = out_limit;
}

/**
 * Initialize a pid unit
 * @param pid
 * @param kp
 * @param ki
 * @param kd
 * @param i_limit
 * @param out_limit
 */
void pid_init(pid_unit *pid, float kp, float ki, float kd, float i_limit, float out_limit) {
    pid_change(pid, kp, ki, kd, i_limit, out_limit);
    pid->p_out = pid->i_out = pid->d_out = pid->out = 0;
    pid->error[0] = pid->error[1] = 0.0;
}

/**
 * Perform pid calculation
 * @param pid
 * @param now
 * @param target
 * @return the calculation result
 */
float pid_calc(pid_unit *pid, float now, float target) {
//    pid->now = now;
//    pid->target = target;
    pid->error[1] = pid->error[0];
    pid->error[0] = target - now;

    //Position PID
    pid->p_out = pid->kp * pid->error[0];
    pid->i_out += pid->ki * pid->error[0];
    pid->d_out = pid->kd * (pid->error[0] - pid->error[1]);

    ABS_LIMIT(pid->i_out, pid->i_limit);
    pid->out = pid->p_out + pid->i_out + pid->d_out;
    ABS_LIMIT(pid->out, pid->out_limit);

    return pid->out;
}
