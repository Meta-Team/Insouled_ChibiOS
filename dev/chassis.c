//
// Created by 岑帜 on 2018/3/18.
//

#include <stdlib.h>
#include "chassis.h"
#include "global.h"

void chassis_ctrl() {
    switch (global_mode) {
        case

            GLOBAL_MODE_SAFETY: {
            chassis.motor_current[0] = 0;
            chassis.motor_current[1] = 0;
            chassis.motor_current[2] = 0;
            chassis.motor_current[3] = 0;
        }

        case

            GLOBAL_MODE_PC: {

        }

        case

            GLOBAL_MODE_REMOTE_CHASSIS: {

            chassis.vx = remote.ch0 * CHASSIS_RC_MAX_SPEED_X;
            chassis.vy = remote.ch1 * CHASSIS_RC_MAX_SPEED_Y;
            chassis.w = remote.ch2 * CHASSIS_RC_MAX_W;

            float rotate_ratio = ((CHASSIS_WHEELBASE + CHASSIS_WHEELTRACK) / 2.0f) / CHASSIS_RADIAN_COEF;
            float rpm_ratio = (60.0f / (CHASSIS_WHEEL_PERIMETER * CHASSIS_DECELE_RATIO));
            chassis.motor_speed[0] = (int)((-chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
            chassis.motor_speed[1] = (int)((chassis.vx - chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
            chassis.motor_speed[2] = (int)((chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);
            chassis.motor_speed[3] = (int)((-chassis.vx + chassis.vy + chassis.w * rotate_ratio) * rpm_ratio);


            int max_motor_speed = 0;
            for (int i = 0; i < 4; i++) {
                if (abs(chassis.motor_speed[i]) > max_motor_speed)
                    max_motor_speed = abs(chassis.motor_speed[i]);
            }

            if (max_motor_speed > CHASSIS_MOTOE_MAX_SPEED) {
                float rate = CHASSIS_MOTOE_MAX_SPEED / max_motor_speed;
                for (int i = 0; i < 4; i++)
                    chassis.motor_speed[i] *= rate;
            }

            for (int i = 0; i < 4; i++) {
                chassis.motor_current[i] = chassis.motor_speed[i] / CHASSIS_MOTOE_MAX_SPEED * CHASSIS_MOTOR_MAX_CURRENT;
            }
        }

        case

            GLOBAL_MODE_REMOTE_GIMBAL: {
            chassis.motor_current[0] = 0;
            chassis.motor_current[1] = 0;
            chassis.motor_current[2] = 0;
            chassis.motor_current[3] = 0;
        }

    }


}
