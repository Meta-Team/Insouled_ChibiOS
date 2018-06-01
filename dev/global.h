//
// Created by liuzikai on 18/03/2018.
//

#ifndef INSOULED_GLOBAL_H
#define INSOULED_GLOBAL_H

#include "hal.h"
#include "ch.h"
#include "chprintf.h"
#include "serial_debug.h"

#include <stdlib.h>


#define INFANTRY1



#define ABS_LIMIT(n, limit) { \
    if (n > limit) n = limit; \
    if (n < -(limit)) n = -(limit); \
}

#define VAL_LIMIT(n, max, min) { \
    if (n > max) n = max; \
    if (n < min) n = min; \
}

#define SIGN(n) (n == 0 ? 0 : (n > 0 ? 1 : -1))
#define EQUAL_ZERO(n) (n < 0.001f && n > -0.001f)

#define LED_G_ON() palClearPad(GPIOF, GPIOF_LED_G)
#define LED_G_OFF() palSetPad(GPIOF, GPIOF_LED_G)
#define LED_G_TOGGLE() palTogglePad(GPIOF, GPIOF_LED_G)
#define LED_R_ON() palClearPad(GPIOE, GPIOE_LED_R)
#define LED_R_OFF() palSetPad(GPIOE, GPIOE_LED_R)
#define LED_R_TOGGLE() palTogglePad(GPIOE, GPIOE_LED_R)

extern int _led_debug_index;
#define LED_DEBUG_TRANSMIT(n) _led_debug_index = n


#endif //INSOULED_GLOBAL_H
