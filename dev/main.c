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

    // Mode handle state machine initialization
    mode_handle_init(); //NOTE: Initialize pc_mode

    // Parameter calculation initialization
    me_arm_init(); //NOTE: Initialize me_arm_mode
    chassis_calc_init(); //NOTE: Based on pc_mode
    gimbal_calc_init(); //NOTE: Based on me_arm_mode
    shoot_calc_init();


    /*** Enter normal operation mode ***/

    mode_handle_thread_init();
    send_thread_init();

    while (true) {
        // Main loop, runs mode handle state machine
        main_control_loop();
        chThdSleepMilliseconds(100);
    }
}
