//
// Created by liuzikai on 2018/4/4.
//

#ifndef INSOULED_CHIBIOS_MOTOR_INTERACTION_H
#define INSOULED_CHIBIOS_MOTOR_INTERACTION_H

#include "global.h"

#include "chassis.h"
#include "gimbal.h"

#include "remote.h"

void motor_can_init(void);

void send_chassis_currents(void);
void send_gimbal_currents(void);

#endif //INSOULED_CHIBIOS_MOTOR_INTERACTION_H
