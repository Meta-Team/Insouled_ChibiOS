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
static void remoteReceived(UARTDriver *uartp);

static UARTConfig remoteUartConfig = {
        NULL,
        NULL,
        remoteReceived,
        NULL,
        NULL,
        100000,
        USART_CR1_PCE,
        0,
        0,
};

/* Info Layer */

typedef enum  {
    LEVER_UP = 1, //up
    LEVER_MD = 3, //middle
    LEVER_DW = 2, //down
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
