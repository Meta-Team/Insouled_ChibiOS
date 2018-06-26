#include "serial_debug.h"

static SerialConfig debugSerialConfig = {
        9600,
        0,
        USART_CR2_STOP1_BITS | USART_CR2_LINEN,
        0
};

void serial_debug_init() {
    palSetPadMode(GPIOG, 14, PAL_MODE_ALTERNATE(8));
    palSetPadMode(GPIOG, 9, PAL_MODE_ALTERNATE(8));
    sdStart(&DEBUG_UART_PORT, &debugSerialConfig);
}
