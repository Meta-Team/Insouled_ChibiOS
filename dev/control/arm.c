#include "arm.h"

#include "mode_handle.h"
#include "remote.h"
#include "gimbal.h"

me_arm_mode_t me_arm_mode;

static PWMConfig me_arm_pwmcfg = {
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

static inline int me_arm_angle_to_pwm(int angle) {
    return 250 + angle * 1000.0f / 180.0f;
}

static inline void me_arm_currents(int angle) {
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_LEFT,
                     PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM,
                                             me_arm_angle_to_pwm(angle)));
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_RIGHT,
                     PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM,
                                             me_arm_angle_to_pwm(180 - angle)));
}

static inline void me_arm_pad(int speed) {
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_CENTER,
                     PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM,
                                             me_arm_angle_to_pwm(speed)));
}

me_arm_mode_t new_me_arm_mode;

void me_arm_handle(void) {
    switch (global_mode) {
        case GLOBAL_MODE_PC:
            if (mouse.press_left && !mouse.press_right) {
                me_arm_currents(ME_ARM_ANGLE_CLOSE);
                me_arm_pad(0);
                new_me_arm_mode = ME_ARM_MODE_EMPTY;
            } else if (mouse.press_right && !mouse.press_left) {
                me_arm_currents(ME_ARM_ANGLE_OPEN);
                me_arm_pad(180);
                new_me_arm_mode = ME_ARM_MODE_EMPTY;
            } else {
                me_arm_currents(ME_ARM_ANGLE_DEFAULT);
                me_arm_pad(90);
                if (keyboard.press_b) {
                    new_me_arm_mode = ME_ARM_MODE_PICKED;
                }
            }
            if (me_arm_mode != new_me_arm_mode) {
                me_arm_mode = new_me_arm_mode;
                gimbal_init_pid_based_on_me_arm_mode();
            }

            break;
        case GLOBAL_MODE_REMOTE_ME_ARM:
            if (remote.ch3 > 0) {
                me_arm_currents(
                        remote.ch3 * (ME_ARM_ANGLE_CLOSE - ME_ARM_ANGLE_DEFAULT)
                        + ME_ARM_ANGLE_DEFAULT);
            } else {
                me_arm_currents(
                        remote.ch3 * (ME_ARM_ANGLE_DEFAULT - ME_ARM_ANGLE_OPEN)
                        + ME_ARM_ANGLE_DEFAULT);
            }
            me_arm_pad((1.0f - remote.ch1) * 90.0f);
            break;
    }
}

void me_arm_init() {

    palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 13, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 14, PAL_MODE_ALTERNATE(2));

    pwmStart(&ME_ARM_PWM_TIM, &me_arm_pwmcfg);
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_LEFT,
                     PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, 750));
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_RIGHT,
                     PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, 750));
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_CENTER,
                     PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, 750));

    me_arm_mode = ME_ARM_MODE_EMPTY;

}