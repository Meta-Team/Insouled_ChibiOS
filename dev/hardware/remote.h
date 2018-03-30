//
// Created by Lan Tian on 18/03/2018.
//

#ifndef INSOULED_REMOTE_H
#define INSOULED_REMOTE_H

#include "ch.h"
#include "hal.h"

#define REMOTE_UART_PORT UARTD1
#define REMOTE_DATA_SIZE 18
char remoteData[REMOTE_DATA_SIZE];

void remoteInit(void);
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

#endif //INSOULED_REMOTE_H
