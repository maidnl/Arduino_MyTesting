
/* -------------------------------------------------------------------------- */
/* FILE NAME:
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include <Wire.h>

/* --- CONFIGURAZIONE --- */
#include "atest_wire_config.h"
#include "atestlib.h"

uint8_t rx_buffer[RX_BUFFER_DIM];

static bool rx_event_flag = false;
static bool tx_event_flag = false;
static int rx_num = 0;

/* _____________________________________________________________RECEIVE EVENT */
void receive_event(int n) {
  rx_num = n;
  rx_event_flag = true;

  for (int i = 0; i < n && i < RX_BUFFER_DIM; i++) {
    int r = WIRE.read();
    rx_buffer[i] = r;
  }
}

/* _____________________________________________________________REQUEST EVENT */
void request_event() {
  if (test_check_buffer<uint8_t>((uint8_t *)rx_buffer, (uint8_t *)req,
                                 sizeof(req)) == 0) {
    tx_event_flag = true;
    WIRE.write((uint8_t *)expected, sizeof(expected));
  }
}

/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  alogln("*** TEST I2C SLAVE Sketch ***");
  i2c_init<Wire_t>(WIRE, SLAVE_ADDRESS);

  WIRE.onReceive(receive_event);
  WIRE.onRequest(request_event);
}

/* _______________________________________________________________test_task() */
void test_task() {
  if (rx_event_flag) {
    rx_event_flag = false;
    alog("[LOG]: I2C Slave received: ");
    for (int i = 0; i < rx_num && i < RX_BUFFER_DIM; i++) {
      alog("0x%02X ", rx_buffer[i]);
    }
    alogln("");
  }

  if (tx_event_flag) {
    tx_event_flag = false;
    alog("[LOG]: I2C Slave sent answer: ");
    for (uint32_t i = 0; i < sizeof(req); i++) {
      alog("0x%02X ", req[i]);
    }
    alogln("");
  }
}

/* ___________________________________________________________________ loop() */
void loop() { test_task(); }
