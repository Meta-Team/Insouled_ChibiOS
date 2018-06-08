#ifndef INSOULED_CHIBIOS_ARM_H
#define INSOULED_CHIBIOS_ARM_H

#include "../global.h"
#include "mode_handle.h"
#include "../info_interaction/remote.h"

#define ENGINEERING_ARM_PWM_TIM PWMD4
#define ENGINEERING_ARM_PWM_LEFT 0
#define ENGINEERING_ARM_PWM_RIGHT 1
#define ENGINEERING_ARM_PWM_CENTER 2

void engineering_arm_calculate(void);
inline int engineering_arm_angle_to_pwm(int angle);
void engineering_arm_currents(int angle);
void engineering_arm_init(void);

#endif