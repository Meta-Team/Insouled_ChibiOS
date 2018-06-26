//
// Created by liuzikai on 2018/4/4.
//

#include <can.h>
#include <component_handle/gimbal.h>
#include <component_handle/chassis.h>
#include <hal_can_lld.h>

#include "remote.h"
#include "mode_handle.h"

/* CAN Configurations */
static const CANConfig cancfg = {
        CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
        CAN_BTR_SJW(0) | CAN_BTR_TS2(3) |
        CAN_BTR_TS1(8) | CAN_BTR_BRP(2)
};


/** CAN feedbacks are high frequency, but calculations are relatively slow.
 * To save calculation resource, "decode" functions only retrieve data,
 * and following processing will be triggered before calculations with
 * "pre_process" functions. */



/**
 * Decode and store raw data of chassis motors.
 * @param rxmsg
 */
static inline void decode_chassis_feedback(CANRxFrame *rxmsg) {
    chSysLock();
    chassis_motor_t *motor = &chassis.motor[rxmsg->SID - 0x201];
    motor->actual_angle_raw = rxmsg->data16[0];
    motor->actual_rpm = (int16_t) rxmsg->data16[1];
    motor->actual_current = (int16_t) rxmsg->data16[2];
    motor->actual_temperature = rxmsg->data8[6];
    chSysUnlock();
}

/**
 * Decode and store raw data of gimbal motors, and handle multiple cycles.
 * A counter is used to reduce calculation frequency
 * @param rxmsg
 */
int gimbal_feedback_counter = 0;
static inline void decode_gimbal_feedback(CANRxFrame *rxmsg) {
    chSysLock();
    gimbal_motor_t *motor = &gimbal.motor[rxmsg->SID - 0x205];
    motor->actual_angle_raw = rxmsg->data16[0];

    // Regarding that motors won't go over a cycle during 20 times of feedback
    if (gimbal_feedback_counter > 20) {

        if (motor->actual_angle_raw + GIMBAL_MOTOR_MAX_DELTA_ANGLE < motor->last_actual_angle_raw) {
            // 8192 -> 0
            motor->round_count++;
        } else if (motor->actual_angle_raw > motor->last_actual_angle_raw + GIMBAL_MOTOR_MAX_DELTA_ANGLE) {
            // 0 -> 8192
            motor ->round_count--;
        }
        motor->last_actual_angle_raw = motor->actual_angle_raw;

        gimbal_feedback_counter = 0;

#ifdef DEBUG_FEEDBACK_GIMBAL_ORIG_ANGLE
        if (motor_id == GIMBAL_MOTOR_YAW) {
        print("[GIMBAL_ORIG] Yaw = %d\n", (int)feedback_angle_orig);
    } else if (motor_id == GIMBAL_MOTOR_ROLL) {
        print("[GIMBAL_ORIG] Pit = %d\n", (int)feedback_angle_orig);
    }
#endif

    } else {
        gimbal_feedback_counter++;
    }
    chSysUnlock();

//    int motor_id = rxmsg->SID - 0x205;
//    uint16_t feedback_angle_orig = (rxmsg->data8[0] << 8 | rxmsg->data8[1]);
//
//
//
//    float angle = ((float)feedback_angle_orig - (float)gimbal_fi_orig[motor_id]) * 360.0f / 8192.0f;
//
//    if (gimbal_fi_orig[motor_id] > 4096 && feedback_angle_orig < gimbal_fi_orig[motor_id] - 4096) //Case I, Red Range
//        angle += 360.0f;
//    if (gimbal_fi_orig[motor_id] < 4096 && feedback_angle_orig > gimbal_fi_orig[motor_id] + 4096) //Case II, Red Range
//        angle -= 360.0f;
//
//    int16_t new_angle = (int16_t) (-1.0f * angle + 360.0f * gimbal.motor[motor_id].actual_angle_base_round);
//    if (new_angle - gimbal.motor[motor_id].actual_angle_last > GIMBAL_MOTOR_MAX_DELTA_ANGLE) {
//        // From - to + across 180
//        gimbal.motor[motor_id].actual_angle_base_round--;
//        new_angle -= 360;
//    }
//    if (new_angle - gimbal.motor[motor_id].actual_angle_last < -GIMBAL_MOTOR_MAX_DELTA_ANGLE) {
//        // From + to - across 180
//        gimbal.motor[motor_id].actual_angle_base_round++;
//        new_angle += 360;
//    }
//    gimbal.motor[motor_id].actual_angle_last = gimbal.motor[motor_id].actual_angle;
//    gimbal.motor[motor_id].actual_angle = new_angle;
}


static THD_WORKING_AREA(can_rx1_wa, 256);

static THD_FUNCTION(can_rx, p) {
    CANDriver *canp = p;
    event_listener_t el;
    CANRxFrame rxmsg;

    chRegSetThreadName("can_receiver");
    chEvtRegister(&canp->rxfull_event, &el, 0);
    while (!chThdShouldTerminateX()) {
        if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(10)) == 0) continue;
        while (canReceive(canp, CAN_ANY_MAILBOX, &rxmsg, TIME_IMMEDIATE) == MSG_OK) {
            switch (rxmsg.SID) {
                case 0x201:
                case 0x202:
                case 0x203:
                case 0x204:
                    decode_chassis_feedback(&rxmsg);
                    break;
                case 0x205:
                case 0x206:
                    decode_gimbal_feedback(&rxmsg);
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * Crop and transmit chassis motor currents
 */
void send_chassis_currents(void) {

    ABS_LIMIT(chassis.motor[0].target_current, CHASSIS_MOTOR_MAX_CURRENT);
    ABS_LIMIT(chassis.motor[1].target_current, CHASSIS_MOTOR_MAX_CURRENT);
    ABS_LIMIT(chassis.motor[2].target_current, CHASSIS_MOTOR_MAX_CURRENT);
    ABS_LIMIT(chassis.motor[3].target_current, CHASSIS_MOTOR_MAX_CURRENT);

    CANTxFrame txmsg;
    txmsg.IDE = CAN_IDE_STD;
    txmsg.SID = 0x200;
    txmsg.RTR = CAN_RTR_DATA;
    txmsg.DLC = 0x08;
    txmsg.data16[0] = (uint16_t) chassis.motor[0].target_current;
    txmsg.data16[1] = (uint16_t) chassis.motor[1].target_current;
    txmsg.data16[2] = (uint16_t) chassis.motor[2].target_current;
    txmsg.data16[3] = (uint16_t) chassis.motor[3].target_current;

//    txmsg.data8[0] = (uint8_t) (chassis.motor[0].target_current >> 8);
//    txmsg.data8[1] = (uint8_t) chassis.motor[0].target_current;
//    txmsg.data8[2] = (uint8_t) (chassis.motor[1].target_current >> 8);
//    txmsg.data8[3] = (uint8_t) chassis.motor[1].target_current;
//    txmsg.data8[4] = (uint8_t) (chassis.motor[2].target_current >> 8);
//    txmsg.data8[5] = (uint8_t) chassis.motor[2].target_current;
//    txmsg.data8[6] = (uint8_t) (chassis.motor[3].target_current >> 8);
//    txmsg.data8[7] = (uint8_t) chassis.motor[3].target_current;
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
}

/**
 * Crop and transmit gimbal and shoot mechanism motor currents
 */
void send_gimbal_shoot_currents(void) {

    ABS_LIMIT(gimbal.motor[GIMBAL_MOTOR_YAW].target_current, GIMBAL_MOTOR_MAX_CURRENT);
    ABS_LIMIT(gimbal.motor[GIMBAL_MOTOR_ROLL].target_current, GIMBAL_MOTOR_MAX_CURRENT);

    CANTxFrame txmsg;
    txmsg.IDE = CAN_IDE_STD;
    txmsg.SID = 0x1FF;
    txmsg.RTR = CAN_RTR_DATA;
    txmsg.DLC = 0x08;
    txmsg.data16[0] = (uint16_t) gimbal.motor[GIMBAL_MOTOR_YAW].target_current;
    txmsg.data16[1] = (uint16_t) gimbal.motor[GIMBAL_MOTOR_ROLL].target_current;
    txmsg.data16[2] = 0;
    txmsg.data16[3] = 0;
//    txmsg.data8[0] = (uint8_t) (gimbal.motor[GIMBAL_MOTOR_YAW].target_current >> 8);
//    txmsg.data8[1] = (uint8_t) gimbal.motor[GIMBAL_MOTOR_YAW].target_current;
//    txmsg.data8[2] = (uint8_t) (gimbal.motor[GIMBAL_MOTOR_ROLL].target_current >> 8);
//    txmsg.data8[3] = (uint8_t) gimbal.motor[GIMBAL_MOTOR_ROLL].target_current;
//    txmsg.data8[4] = (uint8_t) (shoot_mechanism.stir_current >> 8);
//    txmsg.data8[5] = (uint8_t) shoot_mechanism.stir_current;
//    txmsg.data8[6] = (uint8_t) (zero_current >> 8);
//    txmsg.data8[7] = (uint8_t) zero_current;
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
}


/**
 * Open ports and start the receiver thread
 */
void can_init(void) {
    /*
     * Activates the CAN drivers 1 and 2.
     */
    canStart(&CAND1, &cancfg);

    /*
     * Starting the receiver threads.
     */
    chThdCreateStatic(can_rx1_wa, sizeof(can_rx1_wa), NORMALPRIO - 1, can_rx, &CAND1);
}