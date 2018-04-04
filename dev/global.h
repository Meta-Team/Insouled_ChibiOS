//
// Created by liuzikai on 18/03/2018.
//

#ifndef INSOULED_GLOBAL_H
#define INSOULED_GLOBAL_H

#include <stdlib.h>

#define ABS_LIMIT(n, limit) { \
    if (n > limit) n = limit; \
    if (n < -limit) n = -limit; \
}

#define LED_G_ON() palClearPad(GPIOF, GPIOF_LED_G)
#define LED_G_OFF() palSetPad(GPIOF, GPIOF_LED_G)
#define LED_R_ON() palClearPad(GPIOE, GPIOE_LED_R)
#define LED_R_OFF() palSetPad(GPIOE, GPIOE_LED_R)


#endif //INSOULED_GLOBAL_H
