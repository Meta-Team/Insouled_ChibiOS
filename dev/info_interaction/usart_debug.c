#include "usart_debug.h"

static UARTConfig debugUsartConfig = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        9600,
        0,
        USART_CR2_LINEN,
        0,
};

void txend1(UARTDriver *uartp) {

    (void)uartp;
}

void txend2(UARTDriver *uartp) {

    (void)uartp;
}

void rxend(UARTDriver *uartp) {

    (void)uartp;
}

void rxchar(UARTDriver *uartp, uint16_t c) {

    (void)uartp;
    (void)c;
}

void rxerr(UARTDriver *uartp, uartflags_t e) {

    (void)uartp;
    (void)e;
}


static THD_WORKING_AREA(debug_usart_wa, 256);

static THD_FUNCTION(debug_usart, p) {
    (void)p;
    chRegSetThreadName("usart_test");

    static const char teststr[] = "Hello World";
    size_t sz = 12;

    while (true) {
        uartStartSend(&DEBUG_UART_PORT, sz, teststr);
        chThdSleepMilliseconds(1000);
        LED_R_TOGGLE();
    }
}

void usart_debug_init() {
    palSetPadMode(GPIOG, 14, PAL_MODE_ALTERNATE(8));
    palSetPadMode(GPIOG, 9, PAL_MODE_ALTERNATE(8));
    uartStart(&DEBUG_UART_PORT, &debugUsartConfig);
    //uartStartReceive(&DEBUG_UART_PORT, REMOTE_DATA_SIZE, remoteData);

    chThdCreateStatic(debug_usart_wa, sizeof(debug_usart_wa), HIGHPRIO,
                      debug_usart, NULL);
}
