/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"

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
  chRegSetThreadName("receiver");
  chEvtRegister(&cip->canp->rxfull_event, &el, 0);
  while (true) {
    if (chEvtWaitAnyTimeout(ALL_EVENTS, MS2ST(10)) == 0)
      continue;
    while (canReceive(cip->canp, CAN_ANY_MAILBOX,
                      &rxmsg, TIME_IMMEDIATE) == MSG_OK) {
      // Process message.
      palTogglePad(GPIOF, GPIOF_LED_G);       /* Yellow.  */
      palTogglePad(GPIOE, GPIOE_LED_R);       /* Yellow.  */
    }
  }
  chEvtUnregister(&CAND1.rxfull_event, &el);
}


void setMotorSpeed(int16_t ch1, int16_t ch2, int16_t ch3, int16_t ch4) {
  CANTxFrame txmsg;
  txmsg.IDE = CAN_IDE_STD;
  txmsg.SID = 0x200;
  txmsg.RTR = CAN_RTR_DATA;
  txmsg.DLC = 0x08;
  txmsg.data8[0] = (uint8_t) (ch1 >> 8);
  txmsg.data8[1] = (uint8_t) ch1;
  txmsg.data8[2] = (uint8_t) (ch2 >> 8);
  txmsg.data8[3] = (uint8_t) ch2;
  txmsg.data8[4] = (uint8_t) (ch3 >> 8);
  txmsg.data8[5] = (uint8_t) ch3;
  txmsg.data8[6] = (uint8_t) (ch4 >> 8);
  txmsg.data8[7] = (uint8_t) ch4;
  canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(10));
}

static THD_FUNCTION(can_tx, p) {

  (void)p;
  chRegSetThreadName("transmitter");

  while (true) {
    setMotorSpeed(400, 400, 400, 400);
chThdSleepMilliseconds(200);
  }
}


/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

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
  chThdCreateStatic(can_rx1_wa, sizeof(can_rx1_wa), NORMALPRIO + 7,
                    can_rx, (void *)&can1);
  chThdCreateStatic(can_rx2_wa, sizeof(can_rx2_wa), NORMALPRIO + 7,
                    can_rx, (void *)&can2);
  chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), NORMALPRIO + 7,
                    can_tx, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */

    sdStart(&SD2, NULL);
    palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
    palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

    while (true) {
        /*palSetPad(GPIOF, GPIOF_LED_G);
        palSetPad(GPIOE, GPIOE_LED_R);
        chThdSleepMilliseconds(500);
        palClearPad(GPIOF, GPIOF_LED_G);
        palClearPad(GPIOE, GPIOE_LED_R);
        chThdSleepMilliseconds(500);*/
  }
  return 0;
}
