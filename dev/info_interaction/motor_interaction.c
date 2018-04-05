//
// Created by liuzikai on 2018/4/4.
//

#include <control/chassis.h>
#include "motor_interaction.h"

struct can_instance {
    CANDriver     *canp;
    uint32_t      led;
};

static const struct can_instance can1 = {&CAND1, GPIOD_LED5};
static const struct can_instance can2 = {&CAND2, GPIOD_LED3};


/*
 * Internal loopback mode, 500KBaud, automatic wakeup, automatic recover
 * from abort mode.
 * See section 22.7.7 on the STM32 reference manual.
 */

static const CANConfig cancfg = {
        CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
        CAN_BTR_SJW(0) | CAN_BTR_TS2(3) |
        CAN_BTR_TS1(8) | CAN_BTR_BRP(2)
};

static THD_WORKING_AREA(can_rx1_wa, 256);
static THD_WORKING_AREA(can_rx2_wa, 256);
static THD_WORKING_AREA(can_tx_wa, 256);

static THD_FUNCTION(can_rx, p) {
  struct can_instance *cip = p;
  event_listener_t el;
  CANRxFrame rxmsg;

  (void)p;
  chRegSetThreadName("can_receiver");
  chEvtRegister(&cip->canp->rxfull_event, &el, 0);
  while (true) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(10)) == 0)
      continue;
    while (canReceive(cip->canp, CAN_ANY_MAILBOX,
                      &rxmsg, TIME_IMMEDIATE) == MSG_OK) {
      // Process message.
      //palTogglePad(GPIOF, GPIOF_LED_G);       /* Yellow.  */
      //palTogglePad(GPIOE, GPIOE_LED_R);       /* Yellow.  */
    }
  }
  chEvtUnregister(&CAND1.rxfull_event, &el);
}


void set_chassis_currents(void) {

#ifndef MANUAL_DEBUG
  ABS_LIMIT_FEEDBACK(chassis.motor_current[0], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
  ABS_LIMIT_FEEDBACK(chassis.motor_current[1], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
  ABS_LIMIT_FEEDBACK(chassis.motor_current[2], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
  ABS_LIMIT_FEEDBACK(chassis.motor_current[3], CHASSIS_MOTOR_MAX_CURRENT, , LED_R_ON());
#else
  ABS_LIMIT(chassis.motor_current[0], CHASSIS_MOTOR_MAX_CURRENT);
  ABS_LIMIT(chassis.motor_current[1], CHASSIS_MOTOR_MAX_CURRENT);
  ABS_LIMIT(chassis.motor_current[2], CHASSIS_MOTOR_MAX_CURRENT);
  ABS_LIMIT(chassis.motor_current[3], CHASSIS_MOTOR_MAX_CURRENT);
#endif

  CANTxFrame txmsg;
  txmsg.IDE = CAN_IDE_STD;
  txmsg.SID = 0x200;
  txmsg.RTR = CAN_RTR_DATA;
  txmsg.DLC = 0x08;
  txmsg.data8[0] = (uint8_t) (chassis.motor_current[0] >> 8);
  txmsg.data8[1] = (uint8_t) chassis.motor_current[0];
  txmsg.data8[2] = (uint8_t) (chassis.motor_current[1] >> 8);
  txmsg.data8[3] = (uint8_t) chassis.motor_current[1];
  txmsg.data8[4] = (uint8_t) (chassis.motor_current[2] >> 8);
  txmsg.data8[5] = (uint8_t) chassis.motor_current[2];
  txmsg.data8[6] = (uint8_t) (chassis.motor_current[3] >> 8);
  txmsg.data8[7] = (uint8_t) chassis.motor_current[3];
  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
}

static THD_FUNCTION(can_tx, p) {

  (void)p;
  chRegSetThreadName("can_transmitter");

  while (true) {
    set_chassis_currents();
    chThdSleepMilliseconds(200);
  }
}

void motorCanInit(void) {
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
                      can_rx, (void *)&can1);
    chThdCreateStatic(can_rx2_wa, sizeof(can_rx2_wa), NORMALPRIO + 7,
                      can_rx, (void *)&can2);
    chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), NORMALPRIO + 7,
                      can_tx, NULL);
}