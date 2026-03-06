/* -------------------------------------------------------------------------- */
/* FILE NAME:   Toggle_I2C_master_pwm.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:
   DESCRIPTION: Test sketch to verift the possibility to switch between I2C and
                PWM on the same pins
   NOTES:       To be used with slave.ino - which will answer when the requested
                by the master */
/* -------------------------------------------------------------------------- */
#include "Arduino.h"
#include "Wire.h"
#include "atestlib.h"

/* --- CONFIGURAZIONE --- */
#define I2C_PWM_TASK_ms 1000
#define I2C_MASTER_REQUEST_TASK_ms 1000
#define SWITCHING_TASK_ms 10000

#include "atest_wire_config.h"

uint8_t tx_buffer[TX_BUFFER_DIM];
uint8_t rx_buffer[RX_BUFFER_DIM];

static bool i2c_initialized = false;
static bool i2c_active = true;

/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  alogln("*** TEST I2C Master switching I2C pins to PWM fuction ***");
}

/* ___________________________________________________ Send messages to slave */
void i2c_task() {
  static long start = millis();

  if (i2c_active) {

    if (!i2c_initialized) {
      i2c_init<Wire_t>(WIRE, 0);
      i2c_initialized = true;
    }

    if (millis() - start > I2C_MASTER_REQUEST_TASK_ms) {
      start = millis();
      /* send a message without requesting an answer */
      i2c_send_master<Wire_t>(WIRE, SLAVE_ADDRESS, (uint8_t *)msg, sizeof(msg),
                              nullptr, 0, 0);
      delay(100);
      /* send a request with answer expected */
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
}

/* ______________________________________ Activate PWM on the same pin of I2C */
void pwm_task() {
  static long start = millis();

  if (!i2c_active) {
    if (i2c_initialized) {
      alogln("[LOG]: Calling wire.end()");
      WIRE.end();
      i2c_initialized = false;
    }

    if (millis() - start > I2C_PWM_TASK_ms) {
      start = millis();
      alogln("[LOG]: Activating PWM");
      analogWrite(ARDUINO_WIRE_SDA_PIN, 50);
      // analogWrite(ARDUINO_WIRE_SCL_PIN, 100);
    }
  }
}

/* activate either one of the 2 other tasks i2c or pwm with a certain period */
void switch_task() {
  static long start = millis();

  if (millis() - start > SWITCHING_TASK_ms) {
    start = millis();
    if (!i2c_active) {
      alogln("[LOG]: ACTIVATE I2C TASK");
      i2c_active = true;
    } else {
      alogln("[LOG]: ACTIVATE PWM TASK");
      i2c_active = false;
    }
  }
}

/* ____________________________________________________________________loop() */
void loop() {
  switch_task();
  i2c_task();
  pwm_task();
}
