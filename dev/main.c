#include "main.h"

#include "remote.h"
#include "can.h"

#include "mode_handle.h"
#include "arm.h"
#include "chassis.h"
#include "gimbal.h"

#include "main_control_loop.h"

int main(void) {

    /*** Default Initialization ***/
    //ChibiOS initialization
    halInit();
    chSysInit();

    // Debug initialization
    led_init();
    serial_debug_init();
    debug_thread_init();

    // Communication initialization
    remote_init();
    motor_can_init();

    // Parameter calculation initialization
    chassis_calc_init();
    gimbal_calc_init();
    shoot_calc_init();
    //engineering_arm_init();

    /*** Enter normal operation mode ***/

    // Mode handle state machine initialization
    mode_handle_init();

    mode_handle_thread_init();
    send_thread_init();

    while (true) {
        // Main loop, runs mode handle state machine
        main_control_loop();
        chThdSleepMilliseconds(100);
    }
}
