#include <info_interaction/remote.h>
#include "me_arm.h"

#include "mode_handle.h"
#include "remote.h"
#include "gimbal.h"

me_arm_t me_arm;

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

static inline int angle_to_pwm(int angle) {
    return (int) (250 + angle * 1000.0f / 180.0f);
}

static inline void set_arms_angle(int angle) {
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_LEFT, PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, angle_to_pwm(angle)));
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_RIGHT, PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, angle_to_pwm(180 - angle)));
}

static inline void set_baffle(int speed) {
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_CENTER, PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, angle_to_pwm(speed)));
}

static inline void handle_baffle(void) {
    switch (global_mode) {
        case GLOBAL_MODE_PC:
            if ((me_arm.mode == ME_ARM_MODE_NORMAL || me_arm.mode == ME_ARM_MODE_OPEN) && me_arm.baffle_position > 0) {
                set_baffle(180); //Move backward
                me_arm.baffle_position--;
            } else if (me_arm.mode == ME_ARM_MODE_PICKED && me_arm.baffle_position < ME_ARM_BAFFLE_TARGET_POSITION) {
                set_baffle(0); //Move forward
                me_arm.baffle_position++;
            } else {
                set_baffle(90); //Stop
            }
            break;
        case GLOBAL_MODE_REMOTE_ME_ARM:
            set_baffle((int) ((1.0f - remote.ch1) * 90.0f));
            break;
        default:
            break;
    }
}

static inline void handle_arms(void) {
    switch (global_mode) {
        case GLOBAL_MODE_PC:
            switch (me_arm.mode) {
                case ME_ARM_MODE_NORMAL:
                case ME_ARM_MODE_PICKED:
                    set_arms_angle(ME_ARM_ANGLE_DEFAULT);
                    break;
                case ME_ARM_MODE_OPEN:
                    set_arms_angle(ME_ARM_ANGLE_OPEN);
                    break;
            }
            break;
        case GLOBAL_MODE_REMOTE_ME_ARM:
            if (remote.ch3 > 0) {
                set_arms_angle((int) (remote.ch3 * (ME_ARM_ANGLE_CLOSE - ME_ARM_ANGLE_DEFAULT) + ME_ARM_ANGLE_DEFAULT));
            } else {
                set_arms_angle((int) (remote.ch3 * (ME_ARM_ANGLE_DEFAULT - ME_ARM_ANGLE_OPEN) + ME_ARM_ANGLE_DEFAULT));
            }
            break;
        default:
            break;
    }
}

me_arm_mode_t new_me_arm_mode;
static inline void handle_arm_mode(void) {

    if (global_mode == GLOBAL_MODE_PC) {
        if (mouse.press_left) {
            new_me_arm_mode = ME_ARM_MODE_OPEN;
        } else {
            switch (me_arm.mode) {
                case ME_ARM_MODE_OPEN:
                    new_me_arm_mode = ME_ARM_MODE_NORMAL;
                    break;
                case ME_ARM_MODE_NORMAL:
                    if (keyboard.press_b) {
                        LED_R_ON();
                        new_me_arm_mode = ME_ARM_MODE_PICKED;
                    }
                    break;
                default:
                    break;
            }
        }

        if (abs(me_arm.mode - new_me_arm_mode) > 2) {
            // Enter or leave ME_ARM_MODE_PICKED
            gimbal_init_pid_based_on_me_arm_mode();
        }

        me_arm.mode = new_me_arm_mode;
    }
}

void me_arm_handle(void) {
    handle_arm_mode();
    handle_arms();
    handle_baffle();
}

void me_arm_init() {

    palSetPadMode(GPIOD, 12, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 13, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOD, 14, PAL_MODE_ALTERNATE(2));

    pwmStart(&ME_ARM_PWM_TIM, &me_arm_pwmcfg);
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_LEFT, PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, 750));
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_RIGHT, PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, 750));
    pwmEnableChannel(&ME_ARM_PWM_TIM, ME_ARM_PWM_CENTER, PWM_PERCENTAGE_TO_WIDTH(&ME_ARM_PWM_TIM, 750));

    me_arm.mode = ME_ARM_MODE_NORMAL;

}