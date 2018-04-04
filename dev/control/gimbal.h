// Created by 岑帜 on 2018/3/30.
//

#ifndef INSOULED_GIMBAL_H
#define INSOULED_GIMBAL_H

#define YAW_MAX_ANGLE  90
#define YAW_MIN_ANGLE -90
#define PIT_MAX_ANGLE  10
#define PIT_MIN_ANGLE -10

#define GIMBAL_ANGLE_REMOTE_RATIO 10

#define GIMBAL_YAW_CURRENT_ANGLE_RATIO 1.5
#define GIMBAL_PITCH_CURRENT_ANGLE_RATIO 1.5

#define GIMBAL_MOTOR_MAX_CURRENT 2000 //5000


void gimbal_init();
void gimbal_ctrl();

#endif //INSOULED_GIMBAL_H

