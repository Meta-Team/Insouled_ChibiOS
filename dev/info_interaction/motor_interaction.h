//
// Created by liuzikai on 2018/4/4.
//

#ifndef INSOULED_CHIBIOS_MOTOR_INTERACTION_H
#define INSOULED_CHIBIOS_MOTOR_INTERACTION_H

#include "global.h"

#include "chassis.h"
#include "gimbal.h"

#include "remote.h"

void process_chassis_feedback(CANRxFrame* rxmsg);
void process_gimbal_feedback(CANRxFrame* rxmsg);

void send_chassis_currents(void);
void send_gimbal_currents(void);

void motor_can_init(void);

#endif //INSOULED_CHIBIOS_MOTOR_INTERACTION_H
