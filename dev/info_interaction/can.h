//
// Created by liuzikai on 2018/4/4.
//

#ifndef INSOULED_CHIBIOS_MOTOR_INTERACTION_H
#define INSOULED_CHIBIOS_MOTOR_INTERACTION_H

#include "global.h"

void send_chassis_currents(void);
void send_gimbal_shoot_currents(void);

void can_init(void);

#endif //INSOULED_CHIBIOS_MOTOR_INTERACTION_H
