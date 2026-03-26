/* -------------------------------------------------------------------------- */
/* FILE NAME:   i2c_adc_gpio_a4_a5.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       maidnl74@gmail.com
   DATE:        20260325
   DESCRIPTION: Test sketch to verify switch function on A4 and A5 pin.
                This pins can be:
                - Analog input
                - Digital output
                - I2C
   LICENSE:     Copyright (c) 2026 Daniele Aimo
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "Arduino.h"
#include "Wire.h"
#include "atestlib.h"
#include "config.h"

const char *test_title = "*** nano 33 ble sense switch function on A4 A5";

TaskType_t task_list[] = {USE_TASK_I2C,          USE_TASK_DIGITAL_OUTPUT,
                          USE_TASK_ANALOG_INPUT, USE_TASK_I2C,
                          USE_TASK_ANALOG_INPUT, USE_TASK_DIGITAL_OUTPUT};

static int task_list_num = sizeof(task_list) / sizeof(TaskType_t);

extern uint8_t i2c_rx_buffer[];
extern uint8_t i2c_tx_buffer[];

static bool pin_mode_applied = false;
/* ________________________________________________________________i2c_task() */
void i2c_master_task() {
  static unsigned long time = millis();

  if (execute_task(USE_TASK_I2C, time, AT_WIRE_MASTER_TASK_ms)) {
    /* call wire.begin if not yet done */
    i2c_initialize();
    pin_mode_applied = false;

    /* send a message (do not require an answer) */
    i2c_send_master<Wire_t>(AT_WIRE,               // Used wire
                            AT_WIRE_SLAVE_ADDRESS, // the slave address
                            (uint8_t *)msg,        // the message
                            sizeof(msg),           // the size of the message
                            nullptr, 0, 0);

    /* delay between message and request */
    delay(AT_WIRE_MASTER_DELAY_ms);
    /* send a request with answer expected */
    i2c_send_master<Wire_t>(AT_WIRE,                  // Used wire
                            AT_WIRE_SLAVE_ADDRESS,    // the slave address
                            (uint8_t *)req,           // the message request
                            sizeof(req),              // the size of the request
                            (uint8_t *)i2c_rx_buffer, // where put answer
                            sizeof(expected), // size of expected answer,
                            100);             // timeout in ms

    test_check_i2c_answer<uint8_t>(i2c_rx_buffer, (uint8_t *)expected,
                                   sizeof(expected));
  }
}

void digital_output_task() {
  static unsigned long time = millis();
  static bool status = true;

  if (execute_task(USE_TASK_DIGITAL_OUTPUT, time, AT_DIGITAL_OUT_TASK_ms)) {
    alogln("[LOG]: deinitialize digital pins");
    i2c_deinitialize();
    // i2c_set_initialized(false);

    if (pin_mode_applied == false) {
      alogln("[LOG]: Setting digital pins");

      pinMode(AT_WIRE_SDA_PIN, OUTPUT);
      pinMode(AT_WIRE_SCL_PIN, OUTPUT);
      pin_mode_applied = true;
    }

    if (status) {
      status = false;
      alogln("[LOG]: setting pins HIGH");
      digitalWrite(AT_WIRE_SDA_PIN, HIGH);
      digitalWrite(AT_WIRE_SCL_PIN, HIGH);
    } else {
      status = true;
      alogln("[LOG]: setting pins LOW");
      digitalWrite(AT_WIRE_SDA_PIN, LOW);
      digitalWrite(AT_WIRE_SCL_PIN, LOW);
    }
  }
}

void analog_input_task() {
  static unsigned long time = millis();

  i2c_deinitialize();

  if (execute_task(USE_TASK_ANALOG_INPUT, time, AT_ANALOG_IN_TASK_ms)) {
    int ar = analogRead(AT_WIRE_SDA_PIN);
    alogln("[LOG]: voltage read SDA pin %i", ar);
    ar = analogRead(AT_WIRE_SCL_PIN);
    alogln("[LOG]: voltage read SCL pin %i", ar);
  }
}

/* ___________________________________________________________________setup()
 */
void setup() { atestInit(test_title); }

/* ____________________________________________________________________loop()
 */
void loop() {
  switch_task_manually(task_list_num);
  i2c_master_task();
  digital_output_task();
  analog_input_task();
}
