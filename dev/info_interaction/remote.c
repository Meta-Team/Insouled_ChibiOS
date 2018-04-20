//
// Created by Lan Tian on 18/03/2018.
//

#include <main.h>
#include "remote.h"

char remoteData[REMOTE_DATA_SIZE];

remote_info_t remote;

void remote_init(void) {
    palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
    uartStart(&REMOTE_UART_PORT, &remoteUartConfig);
    palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(7));
    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);

    remote.right_lever = remote.left_lever = 0;
    remote.ch0 = remote.ch1 = remote.ch2 = remote.ch3 = 0.0f;
}

void remoteReceived(UARTDriver *uartp) {
    (void) uartp;

    chSysLock();
    remote.ch0 = (((remoteData[0] | remoteData[1] << 8) & 0x07FF) - 1024.0f) / 660.0f;
    ABS_LIMIT(remote.ch0, 1.0f);
    remote.ch1 = (((remoteData[1] >> 3 | remoteData[2] << 5) & 0x07FF) - 1024.0f) / 660.0f;
    ABS_LIMIT(remote.ch1, 1.0f);
    remote.ch2 = (((remoteData[2] >> 6 | remoteData[3] << 2 | remoteData[4] << 10) & 0x07FF) - 1024.0f) / 660.0f;
    ABS_LIMIT(remote.ch2, 1.0f);
    remote.ch3 = (((remoteData[4] >> 1 | remoteData[5] << 7) & 0x07FF) - 1024.0f) / 660.0f;
    ABS_LIMIT(remote.ch3, 1.0f);

    remote.left_lever = ((remoteData[5] >> 4) & 0x000C) >> 2;
    remote.right_lever = (remoteData[5] >> 4) & 0x0003;
    chSysUnlock();


    mode_handle();

    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);
}

void pcReceived(UARTDriver *uartp) {
    (void) uartp;

    chSysLock();
    mouse.x = (remoteData[6] | (remoteData[7] << 8)) / 32768.0f;
    mouse.y = (remoteData[8] | (remoteData[9] << 8)) / 32768.0f;
    mouse.z = (remoteData[10] | (remoteData[11] << 8)) / 32768.0f;

    mouse.press_left = remoteData[12];
    mouse.press_right = remoteData[13];

    keyboard = remoteData[14] | remoteData[15] << 8;
    chSysUnlock();


    mode_handle();

    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);
}