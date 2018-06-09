#ifndef INSOULED_CHIBIOS_ARM_H
#define INSOULED_CHIBIOS_ARM_H

#include "global.h"

#define ME_ARM_PWM_TIM PWMD4
#define ME_ARM_PWM_LEFT 0
#define ME_ARM_PWM_RIGHT 1
#define ME_ARM_PWM_CENTER 2

#define ME_ARM_ANGLE_DEFAULT 90
#define ME_ARM_ANGLE_OPEN 45
#define ME_ARM_ANGLE_CLOSE 120

typedef enum _me_arm_mode_t {
    ME_ARM_MODE_EMPTY = 0,
    ME_ARM_MODE_PICKED = 2
} me_arm_mode_t;

extern me_arm_mode_t me_arm_mode;

void me_arm_handle(void);
void me_arm_init(void);

#endif