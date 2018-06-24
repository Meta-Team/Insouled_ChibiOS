//
// Created by liuzikai on 2018/6/1.
//

#include "shoot.h"

shoot_mechanism_t shoot_mechanism;

int stir_cycle_count = 0;
int16_t stir_current = STIR_MOTOR_BASE_CURRENT;

void shoot_calculate(void) {

    switch (global_mode) {
        case GLOBAL_MODE_INIT:
        case GLOBAL_MODE_SAFETY:
        case GLOBAL_MODE_REMOTE_CHASSIS: {
            SHOOT_ZERO_CURRENT();
            break;
        }
        case GLOBAL_MODE_REMOTE_GIMBAL: {
            shoot_mechanism.shoot_speed_pct = (remote.ch3 + 1.0f) / 2.0f;
            shoot_mechanism.stir_enable = (shoot_mechanism.shoot_speed_pct > STIR_MOTOR_TRIGGER_SPEED_PCT);
            break;
        }
        case GLOBAL_MODE_PC: {
            if (mouse.press_left) {
                if (keyboard.press_shift && !keyboard.press_ctrl)
                    shoot_mechanism.shoot_speed_pct = FRICTION_WHEEL_PC_SHIFT_PCT;
                else if (keyboard.press_ctrl && !keyboard.press_shift)
                    shoot_mechanism.shoot_speed_pct = FRICTION_WHEEL_PC_CTRL_PCT;
                else
                    shoot_mechanism.shoot_speed_pct = FRICTION_WHEEL_PC_BASE_PCT;

                shoot_mechanism.stir_enable = true;
            } else {
                shoot_mechanism.stir_enable = false;
            }
            break;
        }
    }

    if (shoot_mechanism.stir_enable) {
        shoot_mechanism.stir_current = stir_current;
        stir_cycle_count++;
        if (stir_cycle_count >= STIR_MOTOR_CHANGE_DIRECTION_CYCLE_COUNT) {
            stir_current = -stir_current;
            stir_cycle_count = 0;
        }
    } else {
        SHOOT_ZERO_CURRENT();
    }

}

void send_shoot_currents(void) {
    pwmEnableChannel(&FRICTION_WHEEL_PWM_TIM, FRICTION_WHEEL_PWM_CHANNEL0,
                     PWM_PERCENTAGE_TO_WIDTH(&FRICTION_WHEEL_PWM_TIM, shoot_mechanism.shoot_speed_pct * 500 + 500));
    pwmEnableChannel(&FRICTION_WHEEL_PWM_TIM, FRICTION_WHEEL_PWM_CHANNEL1,
                     PWM_PERCENTAGE_TO_WIDTH(&FRICTION_WHEEL_PWM_TIM, shoot_mechanism.shoot_speed_pct * 500 + 500));
}

static PWMConfig friction_wheels_pwmcfg = {
        50000,
        1000,
        NULL,
        {
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_DISABLED, NULL},
                {PWM_OUTPUT_DISABLED, NULL}
        },
        0,
        0
};

void shoot_calc_init(void) {
    palSetPadMode(GPIOI, 5, PAL_MODE_ALTERNATE(3));
    palSetPadMode(GPIOI, 6, PAL_MODE_ALTERNATE(3));
    pwmStart(&PWMD8, &friction_wheels_pwmcfg);
}