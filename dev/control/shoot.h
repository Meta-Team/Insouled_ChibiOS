//
// Created by liuzikai on 2018/6/1.
//

#ifndef INSOULED_CHIBIOS_SHOOT_H
#define INSOULED_CHIBIOS_SHOOT_H

#include "../global.h"

#include "mode_handle.h"
#include "../info_interaction/remote.h"

#define FRICTION_WHEEL_PC_BASE_PCT 0.7f
#define FRICTION_WHEEL_PC_SHIFT_PCT  1.0f
#define FRICTION_WHEEL_PC_CTRL_PCT 0.5f

#define FRICTION_WHEEL_PWM_TIM PWMD8
#define FRICTION_WHEEL_PWM_CHANNEL0 0
#define FRICTION_WHEEL_PWM_CHANNEL1 1

#define STIR_MOTOR_BASE_CURRENT 200
#define STIR_MOTOR_TRIGGER_SPEED_PCT 0.1f

typedef struct {
    float shoot_speed_pct;
    int16_t stir_current;
    uint16_t stir_actual_angle_raw;
    int16_t stir_actual_rpm;
} shoot_mechanism_t;
extern shoot_mechanism_t shoot_mechanism;

#define SHOOT_ZERO_CURRENT() { \
    shoot_mechanism.shoot_speed_pct = 0.0f; \
    shoot_mechanism.stir_current = 0; \
}
//TODO: Add stir motor control

void shoot_calculate(void);
void send_shoot_currents(void);
void shoot_calc_init(void);

#endif //INSOULED_CHIBIOS_SHOOT_H
