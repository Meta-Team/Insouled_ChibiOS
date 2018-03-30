//
<<<<<<< HEAD
// Created by 岑帜 on 2018/3/30.
//

#ifndef INSOULED_GIMBAL_H
#define INSOULED_GIMBAL_H

#define YAW_MAX_ANGLE =  90
#define YAW_MIN_ANGLE = -90
#define PIT_MAX_ANGLE =  10
#define PIT_MIN_ANGLE = -10

#define GIMBAL_ANGLE_REMOTE_RATIO 10

#define GIMBAL_YAW_CURRENT_ANGLE_RATIO 1.5


#endif //INSOULED_GIMBAL_H
=======
// Created by liuzikai on 30/03/2018.
//

#ifndef INSOULED_CHIBIOS_GIMBAL_H
#define INSOULED_CHIBIOS_GIMBAL_H

#define GIMBAL_MOTOR_YAW 0
#define GIMBAL_MOTOR_PITCH 1

struct gimbal_motor_t
{
    int angle;
    int current;
} gimbal[2];

#endif //INSOULED_CHIBIOS_GIMBAL_H
>>>>>>> origin/chassis
