#ifndef INSOULED_CHIBIOS_SERIAL_DEBUG_H
#define INSOULED_CHIBIOS_SERIAL_DEBUG_H

#include "global.h"
#include "chprintf.h"

#define DEBUG_UART_PORT SD6

void usart_debug_init(void);
#define print(...) chprintf((BaseSequentialStream*) &DEBUG_UART_PORT, __VA_ARGS__)

#endif