//
// Created by Lan Tian on 18/03/2018.
//

#include <ch.h>
#include "remote.h"

void remoteInit(void) {
    uartStart(&REMOTE_UART_PORT, &remoteUartConfig);
    palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));
    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);
}

void remoteReceived(UARTDriver *uartp) {
    (void) uartp;
    char leftData = ((remoteData[5] >> 4) & 0x000C) >> 2;
    if(leftData & 0x0001) {
        palSetPad(GPIOF, GPIOF_LED_G);
    } else {
        palClearPad(GPIOF, GPIOF_LED_G);
    }
    if(leftData & 0x0002) {
        palSetPad(GPIOE, GPIOE_LED_R);
    } else {
        palClearPad(GPIOE, GPIOE_LED_R);
    }
    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);
}
