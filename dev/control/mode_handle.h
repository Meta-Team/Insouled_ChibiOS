//
// Created by liuzikai on 2018/4/4.
//

#ifndef INSOULED_CHIBIOS_MODE_HANDLE_H
#define INSOULED_CHIBIOS_MODE_HANDLE_H

#include "global.h"

#include "remote.h"

typedef enum _global_mode_t {
    GLOBAL_MODE_INIT = 9,
    GLOBAL_MODE_SAFETY = 0,
    GLOBAL_MODE_PC = 1,
    GLOBAL_MODE_REMOTE_CHASSIS = 2,
    GLOBAL_MODE_REMOTE_GIMBAL = 3,
    GLOBAL_MODE_ENGINEERING_ARM = 4,
} global_mode_t;
extern global_mode_t global_mode;

void handle_global_mode(void);
void mode_handle_init(void);

#endif //INSOULED_CHIBIOS_MODE_HANDLE_H
