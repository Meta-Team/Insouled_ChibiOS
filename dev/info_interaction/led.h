//
// Created by Lan Tian on 2018/4/20.
//

#ifndef INSOULED_CHIBIOS_LED_DEBUG_H
#define INSOULED_CHIBIOS_LED_DEBUG_H

#include "global.h"

extern int _led_debug_index;
extern int _led_debug_count;

void led_init(void);

#define LED_G_ON() palClearPad(GPIOF, GPIOF_LED_G)
#define LED_G_OFF() palSetPad(GPIOF, GPIOF_LED_G)
#define LED_G_TOGGLE() palTogglePad(GPIOF, GPIOF_LED_G)
#define LED_R_ON() palClearPad(GPIOE, GPIOE_LED_R)
#define LED_R_OFF() palSetPad(GPIOE, GPIOE_LED_R)
#define LED_R_TOGGLE() palTogglePad(GPIOE, GPIOE_LED_R)
#define LED_Blink(n) _led_debug_index = n

#endif //INSOULED_CHIBIOS_LED_DEBUG_H
