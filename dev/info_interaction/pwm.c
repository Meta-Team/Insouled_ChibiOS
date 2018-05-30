//
// Created by Lan Tian on 2018/5/30.
//

#include "pwm.h"

static PWMConfig pwmcfg = {
        1000000,                                    /* 10kHz PWM clock frequency.   */
        1000,                                    /* Initial PWM period 1S.       */
        NULL,
        {
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_ACTIVE_HIGH, NULL}
        },
        0,
        0
};

void pwm_init() {
    palSetPadMode(GPIOI, 5, PAL_MODE_ALTERNATE(3));
    pwmStart(&PWMD8, &pwmcfg);
    pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000));
}