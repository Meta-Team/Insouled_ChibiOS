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

    // Debug initialization
    led_debug_init();
    serial_debug_init();

    // Communication initialization
    remote_init();
    motor_can_init();
    pwm_init();

    // Parameter calculation initialization
    chassis_calc_init();
    gimbal_calc_init();

    // Mode handle state machine initialization
    mode_handle_init();

    while (true) {
        // Main loop, runs mode handle state machine
        handle_global_mode();
        chThdSleepMilliseconds(100);
    }
}
