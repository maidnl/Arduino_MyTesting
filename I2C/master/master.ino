/* -------------------------------------------------------------------------- */
/* FILE NAME:   master.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:
   DESCRIPTION: Test sketch for I2C master device
   NOTES:       To be used with slave.ino                                     */
/* -------------------------------------------------------------------------- */
#include "Arduino.h"
#include "Wire.h"
#include "atestlib.h"

/* --- CONFIGURAZIONE --- */

#include "atest_wire_config.h"

uint8_t tx_buffer[TX_BUFFER_DIM];
uint8_t rx_buffer[RX_BUFFER_DIM];

/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  alogln("*** TEST I2C Master Sketch ***");
  i2c_init<Wire_t>(WIRE, 0);
}

/* ___________________________________ Send data to slave, no answer expected */
void write_task() {
  static long start = millis();

  if (millis() - start > WRITE_TASK_PERIOD) {
    start = millis();
    i2c_send_master<Wire_t>(WIRE, SLAVE_ADDRESS, (uint8_t *)msg, sizeof(msg),
                            nullptr, 0, 0);
  }
}

/* ___________________________________ Send request to slave, answer expected */
void read_task() {
  static long start = millis();

  if (millis() - start > READ_TASK_PERIOD) {
    start = millis();
    i2c_send_master<Wire_t>(WIRE, SLAVE_ADDRESS, (uint8_t *)req, sizeof(msg),
                            (uint8_t *)rx_buffer, sizeof(expected), 100);
    if (test_check_buffer<uint8_t>((uint8_t *)rx_buffer, (uint8_t *)expected,
                                   sizeof(expected)) == 0) {
      alogln("[PASSED]: received expected answer");
    } else {
      alogln("[FAILED]: expected answer not correctly received");
    }
  }
}

/* ____________________________________________________________________loop() */
void loop() {
  write_task();
  delay(10);
  read_task();
}
