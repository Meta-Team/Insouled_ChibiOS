//
// Created by liuzikai on 2018/4/4.
//

#include <can.h>
#include <control/shoot.h>

/* CAN Configurations */
static const CANConfig cancfg = {
        CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
        CAN_BTR_SJW(0) | CAN_BTR_TS2(3) |
        CAN_BTR_TS1(8) | CAN_BTR_BRP(2)
};

/* Receive */
void process_chassis_feedback(CANRxFrame *rxmsg) {
    int motor_id = rxmsg->SID - 0x201;
    uint16_t feedback_angle_orig = (rxmsg->data8[0] << 8 | rxmsg->data8[1]);
    chassis.motor[motor_id].actual_angle = feedback_angle_orig / 8192;
    chassis.motor[motor_id].actual_rpm = (int16_t) (rxmsg->data8[2] << 8 | rxmsg->data8[3]);
    chassis.motor[motor_id].actual_current = (rxmsg->data8[4] << 8 | rxmsg->data8[5]);
    chassis.motor[motor_id].actual_temperature = rxmsg->data8[6];
}

/* Receive */
void process_gimbal_feedback(CANRxFrame *rxmsg) {

    int motor_id = rxmsg->SID - 0x205;
    uint16_t feedback_angle_orig = (rxmsg->data8[0] << 8 | rxmsg->data8[1]);

    float angle = ((float)feedback_angle_orig - (float)gimbal_fi_orig[motor_id]) * 360.0f / 8192.0f;

    if (gimbal_fi_orig[motor_id] > 4096 && feedback_angle_orig < gimbal_fi_orig[motor_id] - 4096) //Case I, Red Range
        angle += 360.0f;
    if (gimbal_fi_orig[motor_id] < 4096 && feedback_angle_orig > gimbal_fi_orig[motor_id] + 4096) //Case II, Red Range
        angle -= 360.0f;

    gimbal.motor[motor_id].actual_angle = (int16_t) (-1.0f * angle);

}

/* Receive */
void process_stir_motor_feedback(CANRxFrame *rxmsg) {
    //int motor_id = rxmsg->SID - 0x201;
    uint16_t feedback_angle_orig = (rxmsg->data8[0] << 8 | rxmsg->data8[1]);
    shoot_mechanism.stir_actual_angle = feedback_angle_orig / 8192;
    shoot_mechanism.stir_actual_rpm = (int16_t) (rxmsg->data8[2] << 8 | rxmsg->data8[3]);
    //print("[FEEDBACK] stir_actual_rpm = %d\n", (int)shoot_mechanism.stir_actual_rpm);
}


static THD_WORKING_AREA(can_rx1_wa, 256);
static THD_WORKING_AREA(can_rx2_wa, 256);

static THD_FUNCTION(can_rx, p) {
    CANDriver *canp = p;
    event_listener_t el;
    CANRxFrame rxmsg;

    chRegSetThreadName("can_receiver");
    chEvtRegister(&canp->rxfull_event, &el, 0);
    while (true) {
        if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(10)) == 0) continue;
        while (canReceive(canp, CAN_ANY_MAILBOX, &rxmsg, TIME_IMMEDIATE) == MSG_OK) {
            // Process message.

            switch (rxmsg.SID) {
                case 0x201:
                case 0x202:
                case 0x203:
                case 0x204:
                    process_chassis_feedback(&rxmsg);
                    break;
                case 0x205:
                case 0x206:
                    process_gimbal_feedback(&rxmsg);
                    break;
                case 0x207:
                    process_stir_motor_feedback(&rxmsg);
                    break;
            }
        }
    }
}

/* Transmit */
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
    txmsg.data8[0] = (uint8_t) (chassis.motor[0].target_current >> 8);
    txmsg.data8[1] = (uint8_t) chassis.motor[0].target_current;
    txmsg.data8[2] = (uint8_t) (chassis.motor[1].target_current >> 8);
    txmsg.data8[3] = (uint8_t) chassis.motor[1].target_current;
    txmsg.data8[4] = (uint8_t) (chassis.motor[2].target_current >> 8);
    txmsg.data8[5] = (uint8_t) chassis.motor[2].target_current;
    txmsg.data8[6] = (uint8_t) (chassis.motor[3].target_current >> 8);
    txmsg.data8[7] = (uint8_t) chassis.motor[3].target_current;
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
    canTransmit(&CAND2, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
}

void send_gimbal_shoot_currents(void) {

    ABS_LIMIT(gimbal.motor[GIMBAL_MOTOR_YAW].target_current, GIMBAL_MOTOR_MAX_CURRENT);
    ABS_LIMIT(gimbal.motor[GIMBAL_MOTOR_PIT].target_current, GIMBAL_MOTOR_MAX_CURRENT);

    int16_t zero_current = 0;

    CANTxFrame txmsg;
    txmsg.IDE = CAN_IDE_STD;
    txmsg.SID = 0x1FF;
    txmsg.RTR = CAN_RTR_DATA;
    txmsg.DLC = 0x08;
    txmsg.data8[0] = (uint8_t) (gimbal.motor[GIMBAL_MOTOR_YAW].target_current >> 8);
    txmsg.data8[1] = (uint8_t) gimbal.motor[GIMBAL_MOTOR_YAW].target_current;
    txmsg.data8[2] = (uint8_t) (gimbal.motor[GIMBAL_MOTOR_PIT].target_current >> 8);
    txmsg.data8[3] = (uint8_t) gimbal.motor[GIMBAL_MOTOR_PIT].target_current;
    txmsg.data8[4] = (uint8_t) (shoot_mechanism.stir_current >> 8);
    txmsg.data8[5] = (uint8_t) shoot_mechanism.stir_current;
    print("%d\n", shoot_mechanism.stir_current);
    txmsg.data8[6] = (uint8_t) (zero_current >> 8);
    txmsg.data8[7] = (uint8_t) zero_current;
    canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
    canTransmit(&CAND2, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
}


/* Initialization */

void motor_can_init(void) {
    /*
     * Activates the CAN drivers 1 and 2.
     */
    canStart(&CAND1, &cancfg);
    canStart(&CAND2, &cancfg);

    /*
     * Starting the receiver threads.
     */
    chThdCreateStatic(can_rx1_wa, sizeof(can_rx1_wa), NORMALPRIO + 7,
                      can_rx, &CAND1);
    chThdCreateStatic(can_rx2_wa, sizeof(can_rx2_wa), NORMALPRIO + 7,
                      can_rx, &CAND2);
}