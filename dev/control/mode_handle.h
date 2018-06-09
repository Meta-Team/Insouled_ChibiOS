//
// Created by liuzikai on 2018/4/4.
//

#ifndef INSOULED_CHIBIOS_MODE_HANDLE_H
#define INSOULED_CHIBIOS_MODE_HANDLE_H

#include "global.h"

typedef enum _global_mode_t {
    GLOBAL_MODE_INIT = 9,
    GLOBAL_MODE_SAFETY = 0,
    GLOBAL_MODE_PC = 1,
    GLOBAL_MODE_REMOTE_CHASSIS = 2,
    GLOBAL_MODE_REMOTE_GIMBAL = 3,
    GLOBAL_MODE_REMOTE_ME_ARM = 4
} global_mode_t;
extern global_mode_t global_mode;

typedef enum _pc_mode_t {
    PC_MODE_NORMAL = 0,
    PC_MODE_ENGI = 1
} pc_mode_t;
extern pc_mode_t pc_mode;

void handle_modes(void);
void mode_handle_init(void);

#endif //INSOULED_CHIBIOS_MODE_HANDLE_H
