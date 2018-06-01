#include "main.h"

int main(void) {

    /*** Default Initialization ***/
    //ChibiOS initialization
    halInit();
    chSysInit();

    // Debug initialization
    led_debug_init();
    serial_debug_init();

    // Communication initialization
    remote_init();
    motor_can_init();

    /*** Enter INIT mode ***/
    global_mode = GLOBAL_MODE_INIT;

    // Parameter calculation initialization
    chassis_calc_init();
    gimbal_calc_init();
    shoot_calc_init();

    for(int i = 0; i < 30; i++) {
        main_control_loop();
        chThdSleepMilliseconds(100);
    }

    /*** Enter normal operation mode ***/

    // Mode handle state machine initialization
    mode_handle_init();

    while (true) {
        // Main loop, runs mode handle state machine
        main_control_loop();
        chThdSleepMilliseconds(100);
    }
}
