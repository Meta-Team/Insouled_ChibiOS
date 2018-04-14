/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "main.h"

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
    //TODO: Understand and modify the priority of the thread.
    chThdCreateStatic(led_debug_wa, sizeof(led_debug_wa), NORMALPRIO + 6,
                      led_debug, NULL);
}

/*
 * Application entry point.
 */
int main(void) {

    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    halInit();
    chSysInit();


    sdStart(&SD2, NULL);


    LED_R_OFF();
    LED_G_OFF();
    led_debug_init();

    remote_init();
    mode_handle_init();

    chassis_calc_init();
    gimbal_calc_init();

    motor_can_init();

    while (true) {
        /*palSetPad(GPIOF, GPIOF_LED_G);
        palSetPad(GPIOE, GPIOE_LED_R);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOF, GPIOF_LED_G);
        palClearPad(GPIOE, GPIOE_LED_R);
        chThdSleepMilliseconds(500);*/
    }
    return 0;
}
