//
// Created by liuzikai on 2018/4/4.
//

#include <motor_interaction.h>
#include <main.h>


/* CAN Configurations */
static const CANConfig cancfg = {
        CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
        CAN_BTR_SJW(0) | CAN_BTR_TS2(3) |
        CAN_BTR_TS1(8) | CAN_BTR_BRP(2)
};

/* Receive */
void process_chassis_feedback(CANRxFrame* rxmsg) {
    int motor_id = rxmsg->SID - 0x201;
    uint16_t report_angle = (rxmsg->data8[0] << 8 | rxmsg->data8[1]);
    chassis.motor[motor_id].actual_angle = report_angle * 360 / 8192;
    chassis.motor[motor_id].actual_rpm = (rxmsg->data8[2] << 8 | rxmsg->data8[3]);
    chassis.motor[motor_id].actual_current = (rxmsg->data8[4] << 8 | rxmsg->data8[5]);
    chassis.motor[motor_id].actual_temperature = rxmsg->data8[6];
}

/* Receive */
void process_gimbal_feedback(CANRxFrame* rxmsg) {
    int motor_id = -1;

//    if (rxmsg->SID == 0x205) motor_id = GIMBAL_MOTOR_YAW;
//    else if(rxmsg->SID == 0x206) motor_id = GIMBAL_MOTOR_PIT;
    motor_id = rxmsg->SID - 0x205;

    int new_angle = (int)((rxmsg->data8[0] << 8 | rxmsg->data8[1]));

    if (motor_id == GIMBAL_MOTOR_YAW) {

        if (new_angle <= 5000) gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle = (int)(-0.044 * new_angle + 40);
        else gimbal.motor[GIMBAL_MOTOR_YAW].actual_angle = (int)(-0.044 * new_angle + 400);


    } else if (motor_id == GIMBAL_MOTOR_PIT) {

        if (new_angle <= 3004) gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle = (int)(-0.044 * new_angle - 48);
        else gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle = (int)(-0.044 * new_angle + 312);

        gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle = (int)(-0.044 * new_angle + 304.0);

//        if (gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle > 0 && gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle < 2) LED_G_ON();
//        else LED_G_OFF();
//
//        if (gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle > -2 && gimbal.motor[GIMBAL_MOTOR_PIT].actual_angle < 0) LED_R_ON();
//        else LED_R_OFF();

    }

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
            }
        }
    }
}

/* Transmit */
void send_chassis_currents(void) {

#ifndef MANUAL_DEBUG
  ABS_LIMIT_FEEDBACK(chassis.target_current[0], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
  ABS_LIMIT_FEEDBACK(chassis.target_current[1], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
  ABS_LIMIT_FEEDBACK(chassis.target_current[2], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
  ABS_LIMIT_FEEDBACK(chassis.target_current[3], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
#else
  ABS_LIMIT(chassis.motor[0].target_current, CHASSIS_MOTOR_MAX_CURRENT);
  ABS_LIMIT(chassis.motor[1].target_current, CHASSIS_MOTOR_MAX_CURRENT);
  ABS_LIMIT(chassis.motor[2].target_current, CHASSIS_MOTOR_MAX_CURRENT);
  ABS_LIMIT(chassis.motor[3].target_current, CHASSIS_MOTOR_MAX_CURRENT);
#endif

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
}

void send_gimbal_currents(void) {

#ifndef MANUAL_DEBUG
  ABS_LIMIT_FEEDBACK(gimbal.target_current[GIMBAL_MOTOR_YAW], GIMBAL_MOTOR_MAX_CURRENT, , LED_R_ON());
  ABS_LIMIT_FEEDBACK(gimbal.target_current[GIMBAL_MOTOR_PIT], GIMBAL_MOTOR_MAX_CURRENT, , LED_R_ON());

#else
  ABS_LIMIT(gimbal.motor[GIMBAL_MOTOR_YAW].target_current, GIMBAL_MOTOR_MAX_CURRENT);
  ABS_LIMIT(gimbal.motor[GIMBAL_MOTOR_PIT].target_current, GIMBAL_MOTOR_MAX_CURRENT);
#endif
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
  txmsg.data8[4] = (uint8_t) (zero_current >> 8);
  txmsg.data8[5] = (uint8_t) zero_current;
  txmsg.data8[6] = (uint8_t) (zero_current >> 8);
  txmsg.data8[7] = (uint8_t) zero_current;
  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
}

/*static THD_WORKING_AREA(can_tx_wa, 256);

static THD_FUNCTION(can_tx, p) {

  (void)p;
  chRegSetThreadName("can_transmitter");

  while (true) {
    send_chassis_currents();
    send_gimbal_currents();
    chThdSleepMilliseconds(10);
  }
}*/





/* Initialization */

void motor_can_init(void) {
    /*
   * Activates the CAN drivers 1 and 2.
   */
    palSetPadMode(GPIOD, 0, PAL_MODE_ALTERNATE(9));
    palSetPadMode(GPIOD, 1, PAL_MODE_ALTERNATE(9));
    canStart(&CAND1, &cancfg);
    canStart(&CAND2, &cancfg);

    /*
     * Starting the transmitter and receiver threads.
     */

  //TODO: Understand and modify the priority of the thread.
    chThdCreateStatic(can_rx1_wa, sizeof(can_rx1_wa), NORMALPRIO + 7,
                      can_rx, &CAND1);
    chThdCreateStatic(can_rx2_wa, sizeof(can_rx2_wa), NORMALPRIO + 7,
                      can_rx, &CAND2);
    /*chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), NORMALPRIO + 7,
                      can_tx, NULL);*/
}