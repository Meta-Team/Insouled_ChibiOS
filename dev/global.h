//
// Created by liuzikai on 18/03/2018.
//

#ifndef INSOULED_GLOBAL_H
#define INSOULED_GLOBAL_H

#include "hal.h"
#include "ch.h"
#include <stdlib.h>

#include "led.h"
#include "chprintf.h"
#include "serial_debug.h"

#include "debug.h"

// Vehicle Type
//#define INFANTRY1
#define ENGINEER



#define ABS_LIMIT(n, limit) { \
    if (n > limit) n = limit; \
    if (n < -(limit)) n = -(limit); \
}

#define VAL_LIMIT(n, max, min) { \
    if (n > max) n = max; \
    if (n < min) n = min; \
}

#define SIGN(n) (n == 0 ? 0 : (n > 0 ? 1 : -1))
#define EQUAL_ZERO(n) (n < 0.00001f && n > -0.00001f)


#endif //INSOULED_GLOBAL_H
