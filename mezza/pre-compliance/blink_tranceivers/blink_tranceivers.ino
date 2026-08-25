/* -------------------------------------------------------------------------- */
/* FILE NAME:   blink+tranceivers.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20260825
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#define BLINK_TIME_ms 500
#define TASK_TIME_ms 100

typedef enum { CAN_TASK, RS485_TASK, ETH_TASK, SDCARD_TASK, WIFI_TASK } Task_t;
/* choose only 1 of the following configuration to run the list of task
 * described by the define itself
 * NOTE: the blink task always runs */

// #define USE_CAN_RS485_ETH
//#define USE_CAN_RS485_ETH_SDCARD
#define USE_CAN_RS485_ETH_SDCARD_WIFI
// #define USE_ONLY_WIFI

#if defined(USE_CAN_RS485_ETH)
#include "compliance_can.h"
#include "compliance_eth.h"
#include "compliance_rs485.h"
#elif defined(USE_CAN_RS485_ETH_SDCARD)
#include "compliance_can.h"
#include "compliance_eth.h"
#include "compliance_rs485.h"
#include "compliance_sdcard.h"
#elif defined(USE_CAN_RS485_ETH_SDCARD_WIFI)
#include "compliance_can.h"
#include "compliance_eth.h"
#include "compliance_rs485.h"
#include "compliance_wifi.h"
#include "compliance_sdcard.h"
#elif defined(USE_ONLY_WIFI)
#include "compliance_wifi.h"

#endif
void setup() {
  Serial.begin(115200);
  delay(2000);

  if (Serial)
    Serial.println("### Running setup:");

  pinMode(LED_BUILTIN, OUTPUT);

#if defined(USE_CAN_RS485_ETH)
  init_can();
  init_rs485();
  init_eth();
#elif defined(USE_CAN_RS485_ETH_SDCARD)
  init_can();
  init_rs485();
  init_eth();
  init_sdcard();
#elif defined(USE_CAN_RS485_ETH_SDCARD_WIFI)
  init_can();
  init_rs485();
  init_eth();
  init_sdcard();
  init_wifi();
#elif defined(USE_ONLY_WIFI)
  init_wifi();
#endif
}

void blink_task() {
  static unsigned long t = 0;
  static int status = false;
  if (millis() - t > BLINK_TIME_ms) {
    t = millis();
    if (Serial)
      Serial.println("[TASK]: Blink");
    if (status) {
      status = false;
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      status = true;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

#if defined(USE_CAN_RS485_ETH)
Task_t update_task(Task_t t) {
  Task_t rv = CAN_TASK;
  if (t == CAN_TASK) {
    rv = RS485_TASK;
  } else if (t == RS485_TASK) {
    rv = ETH_TASK;
  } else if (t == ETH_TASK) {
    rv = CAN_TASK;
  }
  return rv;
}
#elif defined(USE_CAN_RS485_ETH_SDCARD)
Task_t update_task(Task_t t) {
  Task_t rv = CAN_TASK;
  if (t == CAN_TASK) {
    rv = RS485_TASK;
  } else if (t == RS485_TASK) {
    rv = ETH_TASK;
  } else if (t == ETH_TASK) {
    rv = SDCARD_TASK;
  } else if (t == SDCARD_TASK) {
    rv = CAN_TASK;
  }
  return rv;
}
#elif defined(USE_CAN_RS485_ETH_SDCARD_WIFI)
Task_t update_task(Task_t t) {
  Task_t rv = CAN_TASK;
  if (t == CAN_TASK) {
    rv = RS485_TASK;
  } else if (t == RS485_TASK) {
    rv = ETH_TASK;
  } else if (t == ETH_TASK) {
    rv = SDCARD_TASK;
  } else if (t == SDCARD_TASK) {
    rv = WIFI_TASK;
  } else if (t == WIFI_TASK) {
    rv = CAN_TASK;
  }
  return rv;
}
#elif defined(USE_ONLY_WIFI)
Task_t update_task(Task_t t) {
  Task_t rv = WIFI_TASK;
  return rv;
}
#endif

void execute_task(Task_t t) {
  if (t == CAN_TASK) {
#if !defined(USE_ONLY_WIFI)
    can_task();
#endif
  } else if (t == RS485_TASK) {
#if !defined(USE_ONLY_WIFI)
    rs485_task();
#endif
  } else if (t == ETH_TASK) {
#if !defined(USE_ONLY_WIFI)
    eth_task();
#endif
  } else if (t == SDCARD_TASK) {
#if defined(USE_CAN_RS485_ETH_SDCARD_WIFI) || defined(USE_CAN_RS485_ETH_SDCARD)
    sdcard_task();
#endif
  } else if (t == WIFI_TASK) {
#if defined(USE_CAN_RS485_ETH_SDCARD_WIFI) || defined(USE_ONLY_WIFI)
    wifi_task();
    delay(500);
#endif
  }
}

void scheduler() {
  static unsigned long t = 0;
#if defined(USE_ONLY_WIFI)
  static Task_t task = WIFI_TASK;
#else
  static Task_t task = CAN_TASK;
#endif
  if (millis() - t > TASK_TIME_ms) {
    t = millis();
    execute_task(task);
    task = update_task(task);
  }
}

void loop() {
  blink_task();
  scheduler();
}
