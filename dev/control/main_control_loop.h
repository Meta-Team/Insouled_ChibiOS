//
// Created by liuzikai on 2018/6/1.
//

#ifndef INSOULED_CHIBIOS_MAIN_CONTROL_LOOP_H
#define INSOULED_CHIBIOS_MAIN_CONTROL_LOOP_H

#include "global.h"

#include "mode_handle.h"

#include "component_handle/me_arm.h"
#include "component_handle/chassis.h"
#include "component_handle/gimbal.h"

#include "can.h"

void main_control_loop(void);
void mode_handle_thread_init(void);

#endif //INSOULED_CHIBIOS_MAIN_CONTROL_LOOP_H
