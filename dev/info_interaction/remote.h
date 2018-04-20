//
// Created by Lan Tian on 18/03/2018.
//

#ifndef INSOULED_REMOTE_H
#define INSOULED_REMOTE_H

#include "global.h"

#include "mode_handle.h"

/* Hardware Layer */
#define REMOTE_UART_PORT UARTD1
#define REMOTE_DATA_SIZE 18

void remote_init(void);

void remoteReceived(UARTDriver *uartp);

/* Info Layer */

typedef enum  {
    LEVER_NULL = 0, // Uninitialized state
    LEVER_UP = 1,   // Up
    LEVER_MD = 3,   // Middle
    LEVER_DW = 2,   // Down
} remote_lever_state_t;

typedef struct {
    remote_lever_state_t left_lever;
    remote_lever_state_t right_lever;

    float ch0; //right horizontal
    float ch1; //right vertical
    float ch2; //left horizontal
    float ch3; //left vertical

} remote_info_t;
extern remote_info_t remote;

#define MAX_VALID_CHANGE 0.3f

#endif //INSOULED_REMOTE_H
