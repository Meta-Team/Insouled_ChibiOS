#include "main.h"

#include "remote.h"
#include "can.h"

#include "mode_handle.h"
#include "me_arm.h"
#include "component_handle/chassis.h"
#include "component_handle/gimbal.h"

#include "main_control_loop.h"

int main(void) {

    /*** Default Initialization ***/
    //ChibiOS initialization
    halInit();
    chSysInit();

    // Debug initialization
    led_init();
    serial_debug_init();

    // Communication initialization
    remote_init();
    can_init();

    // Mode handle state machine initialization
    mode_handle_init();

    // Component handlers initialization
    me_arm_init();
    chassis_calc_init();
    gimbal_calc_init();

    //TODO: add period of GLOBAL_INIT_MODE

    /*** Enter normal operation mode ***/

    mode_handle_thread_init();

    while (true) {
        // Main loop, runs mode handle state machine
        main_control_loop();
        chThdSleepMilliseconds(100);
    }
}
