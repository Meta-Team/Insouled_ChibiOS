//
// Created by liuzikai on 18/03/2018.
//

#ifndef INSOULED_GLOBAL_H
#define INSOULED_GLOBAL_H

#include "hal.h"
#include "ch.h"

#include <stdlib.h>

#define MANUAL_DEBUG

#define ABS_LIMIT(n, limit) { \
    if (n > limit) n = limit; \
    if (n < -(limit)) n = -(limit); \
}

#define ABS_LIMIT_FEEDBACK(n, limit, in_command, out_command) { \
    if (n <= limit && n >= -(limit)) { \
        in_command; \
    } else { \
        if (n > limit) n = limit; \
        else if (n < -(limit)) n = -(limit); \
        out_command; \
    } \
}

#define LED_G_ON() palClearPad(GPIOF, GPIOF_LED_G)
#define LED_G_OFF() palSetPad(GPIOF, GPIOF_LED_G)
#define LED_G_TOGGLE() palTogglePad(GPIOF, GPIOF_LED_G)
#define LED_R_ON() palClearPad(GPIOE, GPIOE_LED_R)
#define LED_R_OFF() palSetPad(GPIOE, GPIOE_LED_R)
#define LED_R_TOGGLE() palTogglePad(GPIOE, GPIOE_LED_R)

extern int _led_debug_index;
#define LED_DEBUG_TRANSMIT(n) _led_debug_index = n


#endif //INSOULED_GLOBAL_H
