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

#define ME_ARM_BAFFLE_TARGET_POSITION 30

typedef enum _me_arm_mode_t {
    ME_ARM_MODE_NORMAL = 0,
    ME_ARM_MODE_OPEN = 1,
    ME_ARM_MODE_PICKED = 9
} me_arm_mode_t;

typedef struct {
    me_arm_mode_t mode;
    int baffle_position;
} me_arm_t;

extern me_arm_t me_arm;

void me_arm_handle(void);
void me_arm_init(void);

#endif