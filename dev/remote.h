//
// Created by liuzikai on 2018/4/4.
//

#ifndef INSOULED_CHIBIOS_REMOTE_H
#define INSOULED_CHIBIOS_REMOTE_H

typedef enum  {
    LEVER_UP = 1, //up
    LEVER_MD = 0, //middle
    LEVER_DW = -1, //down
} remote_lever_state_t;

typedef struct {
    remote_lever_state_t left_lever;
    remote_lever_state_t right_lever;

    float ch0; //right horizontal
    float ch1; //right vertical
    float ch2; //left horizontal
    float ch3; //left vertical

} remote_info_t;
remote_info_t remote;

#endif //INSOULED_CHIBIOS_REMOTE_H
