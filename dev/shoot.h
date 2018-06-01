//
// Created by liuzikai on 2018/6/1.
//

#ifndef INSOULED_CHIBIOS_SHOOT_H
#define INSOULED_CHIBIOS_SHOOT_H

#include "global.h"

#include "mode_handle.h"
#include "remote.h"

#define FRICTION_WHEEL_PC_BASE_PCT 0.7f
#define FRICTION_WHEEL_PC_SHIFT_PCT  1.0f
#define FRICTION_WHEEL_PC_CTRL_PCT 0.5f

#define FRICTION_WHEEL_PWM_TIM PWMD8
#define FRICTION_WHEEL_PWM_CHANNEL0 0
#define FRICTION_WHEEL_PWM_CHANNEL1 1

typedef struct {
    float speed_pct;
} friction_wheels_t;
extern friction_wheels_t friction_wheels;

#define SHOOT_ZERO_CURRENT() { \
    friction_wheels.speed_pct = 0.0f; \
}
//TODO: Add stir motor control

void shoot_calculate(void);
void send_shoot_currents(void);
void shoot_calc_init(void);

#endif //INSOULED_CHIBIOS_SHOOT_H
