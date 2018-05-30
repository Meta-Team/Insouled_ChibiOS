//
// Created by Lan Tian on 2018/5/30.
//

#include "pwm.h"

static PWMConfig pwmcfg = {
        50000,
        1000,
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
    // TIM4
    palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 13, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 14, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 15, PAL_MODE_ALTERNATE(2));
    pwmStart(&PWMD4, &pwmcfg);
    pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 1000));
    pwmEnableChannel(&PWMD4, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 2000));
    pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 3000));
    pwmEnableChannel(&PWMD4, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 4000));

    // TIM5
    palSetPadMode(GPIOH, 10, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOH, 11, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOH, 12, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOI, 0, PAL_MODE_ALTERNATE(2));
    pwmStart(&PWMD5, &pwmcfg);
    pwmEnableChannel(&PWMD5, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, 1000));
    pwmEnableChannel(&PWMD5, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, 2000));
    pwmEnableChannel(&PWMD5, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, 3000));
    pwmEnableChannel(&PWMD5, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, 4000));

    // TIM8
    palSetPadMode(GPIOI, 5, PAL_MODE_ALTERNATE(3));
    palSetPadMode(GPIOI, 6, PAL_MODE_ALTERNATE(3));
    palSetPadMode(GPIOI, 7, PAL_MODE_ALTERNATE(3));
    palSetPadMode(GPIOI, 2, PAL_MODE_ALTERNATE(3));
    pwmStart(&PWMD8, &pwmcfg);
    pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 3000));
    pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 3000));
    pwmEnableChannel(&PWMD8, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 7000));
    pwmEnableChannel(&PWMD8, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 7000));
}