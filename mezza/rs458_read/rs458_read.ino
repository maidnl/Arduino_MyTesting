/* -------------------------------------------------------------------------- */
/* FILE NAME:   rs458_read.ino
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

#define LED_BUILTIN D8
#include "ArduinoRS485.h"
#include "crc.h"
#include <Arduino.h>

#define BAUD_RATE 14745600
// #define BAUD_RATE 7372800
//  #define BAUD_RATE 3686400
//   #define BAUD_RATE 1843200
//   #define BAUD_RATE 921600
#define ADDRESS 1
/* ___________________________________________________________________setup() */
void setup() {

  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("--- RS485 receiver ----");
  Serial.println("BAUD: ");
  Serial.println(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
  RS485.begin(BAUD_RATE);
  RS485.receive();
}

void blink_led() {
  static unsigned long t = 0;
  if (millis() - t > 1000) {
    t = millis();
    static bool st = true;
    if (st) {
      st = false;
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      st = true;
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}

uint8_t rx_buffer[20];
void reset_rx() {
  for (int i = 0; i < 20; i++) {
    rx_buffer[i] = 0xFA;
  }
}

/* ____________________________________________________________________loop()
 */
void loop() {
  static int i = 0;
  static long int t = 0;
  bool rx = false;
  blink_led();
  if (RS485.available()) {
    if (i == 0) {
      t = millis();
    }
    while (RS485.available()) {
      if (i < 20)
        rx_buffer[i++] = RS485.read();
    }
  }
  if (i >= 13) {
    int n = i;
    i = 0;
    if (rx_buffer[0] == ADDRESS) {
      Serial.print("RX ");
      Serial.print(n);
      Serial.print(" ");
      Serial.print("Address: ");
      Serial.print(rx_buffer[0]);
      Serial.print("  Data: ");
      for (int i = 1; i < 10; i++) {
        Serial.print((char)rx_buffer[i]);
      }
      Serial.print(" -> ");
      Serial.print(rx_buffer[10]);
      uint16_t crc = crc16(rx_buffer, 11);
      Serial.print("   | crc calculated: 0x");
      Serial.print(crc, HEX);
      Serial.print(" received: 0x");

      Serial.print(rx_buffer[11], HEX);
      Serial.print(" --- ");
      Serial.print(rx_buffer[12], HEX);
      Serial.print(" ");
      Serial.print((crc & 0xFF00) >> 8, HEX);
      Serial.print(" ");
      Serial.print((crc & 0xFF), HEX);

      if (((crc & 0xFF00) >> 8) == rx_buffer[11] &&
          (crc & 0xFF) == rx_buffer[12]) {
        Serial.println(" OK!");
      } else {
        Serial.println(" WRONG CRC !!!!");
      }

    } else {
      // Serial.println("For other device");
    }
    reset_rx();

  } else if (millis() - t > 500 && i != 0) {
    Serial.println("!!!!! TIMEOUT !!!!!");
    i = 0;
    reset_rx();
  }

  delay(1);
}
