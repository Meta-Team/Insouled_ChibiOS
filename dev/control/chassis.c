//
// Created by 岑帜 on 2018/3/18.
//

#include "chassis.h"

chassis_t chassis;

void chassis_calculate(void) {

    switch (global_mode) {
        case GLOBAL_MODE_SAFETY: {
            CHASSIS_ZERO_CURRENT();
            break;
        }
        case GLOBAL_MODE_PC: {
            CHASSIS_ZERO_CURRENT();
            break;
        }

        case GLOBAL_MODE_REMOTE_CHASSIS: {

            // Calculate car speeds
            chassis.vx = remote.ch1 * CHASSIS_RC_MAX_SPEED_X;
            chassis.vy = -remote.ch0 * CHASSIS_RC_MAX_SPEED_Y;
            chassis.w = remote.ch2 * CHASSIS_RC_MAX_W;

            //Calculate speeds of each wheel
            float rotate_ratio = ((CHASSIS_WHEELBASE + CHASSIS_WHEELTRACK) / 2.0f) / CHASSIS_RADIAN_COEF;
            float rpm_ratio = (60.0f / (CHASSIS_WHEEL_PERIMETER * CHASSIS_DECELE_RATIO));
            chassis.motor_speed[0] = (int) ((-chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
            chassis.motor_speed[1] = (int) ((chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
            chassis.motor_speed[2] = (int) ((chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
            chassis.motor_speed[3] = (int) ((-chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);

            // Limit the max wheel speeds
            int max_motor_speed = 0;
            for (int i = 0; i < 4; i++) {
                if (abs(chassis.motor_speed[i]) > max_motor_speed)
                    max_motor_speed = abs(chassis.motor_speed[i]);
            }

            if (max_motor_speed > CHASSIS_MOTOE_MAX_SPEED) {
                LED_R_TOGGLE();
                float rate = (float) CHASSIS_MOTOE_MAX_SPEED / max_motor_speed;
                for (int i = 0; i < 4; i++)
                    chassis.motor_speed[i] *= rate;
            }

            for (int i = 0; i < 4; i++) {
                chassis.motor_current[i] = (int16_t) ((float) chassis.motor_speed[i] / CHASSIS_MOTOE_MAX_SPEED * CHASSIS_MOTOR_MAX_CURRENT);
            }
            break;
        }

        case GLOBAL_MODE_REMOTE_GIMBAL: {
            CHASSIS_ZERO_CURRENT();
            break;
        }

    }

}

static THD_WORKING_AREA(chassis_calc_wa, 256);

static THD_FUNCTION(chassis_calc, p) {

    (void) p;
    chRegSetThreadName("chassis_calc");

    while (true) {
        chassis_calculate();
        //TODO: Modify the time interval
        chThdSleepMilliseconds(10);
    }
}

void chassis_calc_init(void) {
    //TODO: Understand and modify the priority of the thread.
    chThdCreateStatic(chassis_calc_wa, sizeof(chassis_calc_wa), NORMALPRIO + 6,
                      chassis_calc, NULL);
}