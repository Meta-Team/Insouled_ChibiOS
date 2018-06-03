//
// Created by liuzikai on 2018/4/4.
//

#ifndef INSOULED_CHIBIOS_MOTOR_INTERACTION_H
#define INSOULED_CHIBIOS_MOTOR_INTERACTION_H

#include "global.h"

#include "chassis.h"
#include "gimbal.h"
#include "shoot.h"

#include "remote.h"
#include "mode_handle.h"

void process_chassis_feedback(CANRxFrame* rxmsg);
void process_gimbal_feedback(CANRxFrame* rxmsg);

void send_chassis_currents(void);
void send_gimbal_shoot_currents(void);

void motor_can_init(void);

#endif //INSOULED_CHIBIOS_MOTOR_INTERACTION_H
