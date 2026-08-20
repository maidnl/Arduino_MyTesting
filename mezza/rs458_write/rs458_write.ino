/* -------------------------------------------------------------------------- */
/* FILE NAME:   rs458_write.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       maidnl74@gmail.com
   DATE:        20260817
   DESCRIPTION:
   LICENSE:     Copyright (c) 2026 Daniele Aimo
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#define LED_BUILTIN D4
#define BAUD_RATE 14745600
// #define BAUD_RATE 7372800
// #define BAUD_RATE 3686400
//  #define BAUD_RATE 1843200
//  #define BAUD_RATE 921600
#define SLEEP_TIME_MS 1500

#include "ArduinoRS485.h"
#include "crc.h"
#include <Arduino.h>
void blink_led() {
  static bool st = true;
  if (st) {
    st = false;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    st = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

uint8_t tx_buffer[20];
uint8_t counter[3];

void rs_485_tx(uint8_t address) {

  tx_buffer[0] = address;
  tx_buffer[1] = 'S';
  tx_buffer[2] = 'e';
  tx_buffer[3] = 'n';
  tx_buffer[4] = 't';
  tx_buffer[5] = ' ';
  tx_buffer[6] = 't';
  tx_buffer[7] = 'o';
  tx_buffer[8] = ' ';
  if (address == 1) {
    tx_buffer[9] = 'A';
  } else if (address == 2) {
    tx_buffer[9] = 'B';
  } else if (address == 3) {
    tx_buffer[9] = 'C';
  }
  if (address == 1) {
    tx_buffer[10] = counter[0]++;
  } else if (address == 2) {
    tx_buffer[10] = counter[2]++;
  } else if (address == 3) {
    tx_buffer[10] == counter[3]++;
  }

  uint16_t crc = crc16(tx_buffer, 11);

  tx_buffer[11] = (uint8_t)((crc & 0xFF00) >> 8);
  tx_buffer[12] = (uint8_t)(crc & 0xFF);

  RS485.beginTransmission();
  RS485.write(tx_buffer, 13);
  RS485.endTransmission();
  Serial.println("sent..");
}
/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("--------- RS485 WRITER -------------");
  Serial.println("BAUD: ");
  Serial.println(BAUD_RATE);

  pinMode(LED_BUILTIN, OUTPUT);
  RS485.begin(BAUD_RATE);
}

/* ____________________________________________________________________loop() */
void loop() {
  static int address = 1;
  blink_led();
  rs_485_tx(address);
  address++;
  if (address >= 4) {
    address = 1;
  }
  delay(SLEEP_TIME_MS);
}
