#include <info_interaction/remote.h>
#include "arm.h"

static PWMConfig engineering_arm_pwmcfg = {
        50000,
        1000,
        NULL,
        {
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_ACTIVE_HIGH, NULL},
                {PWM_OUTPUT_DISABLED, NULL}
        },
        0,
        0
};

void engineering_arm_calculate() {
    switch (global_mode) {
        case GLOBAL_MODE_PC:
            if(mouse.press_left) {
                engineering_arm_currents(ENGINEERING_ARM_ANGLE_CLOSE);
                engineering_arm_pad(0);
            } else if(mouse.press_right) {
                engineering_arm_currents(ENGINEERING_ARM_ANGLE_OPEN);
                engineering_arm_pad(180);
            } else {
                engineering_arm_currents(ENGINEERING_ARM_ANGLE_DEFAULT);
                engineering_arm_pad(90);
            }
            break;
        case GLOBAL_MODE_ENGINEERING_ARM:
            if(remote.ch3 > 0) {
                engineering_arm_currents(
                        remote.ch3 * (ENGINEERING_ARM_ANGLE_CLOSE - ENGINEERING_ARM_ANGLE_DEFAULT)
                        + ENGINEERING_ARM_ANGLE_DEFAULT);
            } else {
                engineering_arm_currents(
                        remote.ch3 * (ENGINEERING_ARM_ANGLE_DEFAULT - ENGINEERING_ARM_ANGLE_OPEN)
                        + ENGINEERING_ARM_ANGLE_DEFAULT);
            }
            engineering_arm_pad((1.0f - remote.ch1) * 90.0f);
            break;
    }
}

inline int engineering_arm_angle_to_pwm(int angle) {
    return 250 + angle * 1000.0f / 180.0f;
}

void engineering_arm_currents(int angle) {
    pwmEnableChannel(&ENGINEERING_ARM_PWM_TIM, ENGINEERING_ARM_PWM_LEFT,
                     PWM_PERCENTAGE_TO_WIDTH(&ENGINEERING_ARM_PWM_TIM,
                     engineering_arm_angle_to_pwm(angle)));
    pwmEnableChannel(&ENGINEERING_ARM_PWM_TIM, ENGINEERING_ARM_PWM_RIGHT,
                     PWM_PERCENTAGE_TO_WIDTH(&ENGINEERING_ARM_PWM_TIM,
                     engineering_arm_angle_to_pwm(180 - angle)));
}

void engineering_arm_pad(int speed) {
    pwmEnableChannel(&ENGINEERING_ARM_PWM_TIM, ENGINEERING_ARM_PWM_CENTER,
                     PWM_PERCENTAGE_TO_WIDTH(&ENGINEERING_ARM_PWM_TIM,
                                             engineering_arm_angle_to_pwm(speed)));
}

void engineering_arm_init() {
    palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 13, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 14, PAL_MODE_ALTERNATE(2));
    pwmStart(&ENGINEERING_ARM_PWM_TIM, &engineering_arm_pwmcfg);
    pwmEnableChannel(&ENGINEERING_ARM_PWM_TIM, ENGINEERING_ARM_PWM_LEFT,
                     PWM_PERCENTAGE_TO_WIDTH(&ENGINEERING_ARM_PWM_TIM, 750));
    pwmEnableChannel(&ENGINEERING_ARM_PWM_TIM, ENGINEERING_ARM_PWM_RIGHT,
                     PWM_PERCENTAGE_TO_WIDTH(&ENGINEERING_ARM_PWM_TIM, 750));
    pwmEnableChannel(&ENGINEERING_ARM_PWM_TIM, ENGINEERING_ARM_PWM_CENTER,
                     PWM_PERCENTAGE_TO_WIDTH(&ENGINEERING_ARM_PWM_TIM, 750));

}