#include "led_debug.h"

static THD_WORKING_AREA(led_debug_wa, 256);

int _led_debug_index = 0;
int _led_debug_count = 0;

static THD_FUNCTION(led_debug, p) {
    (void) p;
    chRegSetThreadName("led_debug");

    while (true) {
        if (_led_debug_count % 2 == 0) {
            if (_led_debug_count / 2 < _led_debug_index) {
                LED_R_ON();
            } else {
                LED_R_OFF();
            }
        } else {
            LED_R_OFF();
        }
        _led_debug_count = (_led_debug_count + 1) % 20;
        chThdSleepMilliseconds(150);
    }
}

void led_debug_init(void) {
    // Turn off LED initially
    LED_R_OFF();
    LED_G_OFF();

    chThdCreateStatic(led_debug_wa, sizeof(led_debug_wa), LOWPRIO,
                      led_debug, NULL);
}
