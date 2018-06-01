//
// Created by Lan Tian on 18/03/2018.
//

#include "remote.h"

char remoteData[REMOTE_DATA_SIZE];

remote_info_t remote;
mouse_info_t mouse;
keyboard_info_t keyboard;

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

    remote.left_lever = (remote_lever_state_t) ((remoteData[5] >> 4) & 0x000C) >> 2;
    remote.right_lever = (remote_lever_state_t) (remoteData[5] >> 4) & 0x0003;

    mouse.x = (int16_t)(remoteData[6] | ((int16_t)remoteData[7] << 8)) / 32768.0f;
    ABS_LIMIT(mouse.x, 1.0f);
    mouse.y = (int16_t)(remoteData[8] | ((int16_t)remoteData[9] << 8)) / 32768.0f;
    ABS_LIMIT(mouse.y, 1.0f);
    mouse.z = (int16_t)(remoteData[10] | ((int16_t)remoteData[11] << 8)) / 32768.0f;
    ABS_LIMIT(mouse.z, 1.0f);

    mouse.press_left = (bool)remoteData[12];
    mouse.press_right = (bool)remoteData[13];

    pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, (remote.ch3 + 1) * 250 + 500));
    pwmEnableChannel(&PWMD8, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, (remote.ch3 + 1) * 250 + 500));


    uint16_t keycode = ((uint16_t) remoteData[14] | ((uint16_t)remoteData[15] << 8));

    keyboard.press_w = (bool)(keycode >> 0 & 0x01);
    keyboard.press_s = (bool)(keycode >> 1 & 0x01);
    keyboard.press_a = (bool)(keycode >> 2 & 0x01);
    keyboard.press_d = (bool)(keycode >> 3 & 0x01);
    keyboard.press_ctrl = (bool)(keycode >> 4 & 0x01);
    keyboard.press_shift = (bool)(keycode >> 5 & 0x01);
    keyboard.press_e = (bool)(keycode >> 6 & 0x01);
    keyboard.press_q = (bool)(keycode >> 7 & 0x01);

    chSysUnlock();

    uartStartReceive(&REMOTE_UART_PORT, REMOTE_DATA_SIZE, remoteData);
}