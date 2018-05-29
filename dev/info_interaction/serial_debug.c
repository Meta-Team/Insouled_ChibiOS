#include "serial_debug.h"

static SerialConfig debugSerialConfig = {
        9600,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
};

void usart_debug_init() {
    palSetPadMode(GPIOG, 14, PAL_MODE_ALTERNATE(8));
    palSetPadMode(GPIOG, 9, PAL_MODE_ALTERNATE(8));
    sdStart(&DEBUG_UART_PORT, &debugSerialConfig);
}

int printf(const char *fmt, ...) {
    va_list ap;
    int formatted_bytes;

    va_start(ap, fmt);
    formatted_bytes = chvprintf((BaseSequentialStream*) &DEBUG_UART_PORT, fmt, ap);
    va_end(ap);

    return formatted_bytes;
}