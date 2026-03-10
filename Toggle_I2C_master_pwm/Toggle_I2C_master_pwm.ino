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
#define DIGITAL_INPUT_TASK_ms 1000
#define DIGITAL_OUTPUT_TASk_ms 500
#define SWITCHING_TASK_ms 10000

#include "atest_wire_config.h"

uint8_t tx_buffer[TX_BUFFER_DIM];
uint8_t rx_buffer[RX_BUFFER_DIM];

typedef enum {
  USE_TASK_I2C,
  USE_TASK_PWM,
  USE_TASK_DIGITAL_INPUT,
  USE_TASK_DIGITAL_OUTPUT
} TaskType_t;

TaskType_t task_list[] = {
    USE_TASK_I2C,           USE_TASK_DIGITAL_INPUT,  USE_TASK_I2C,
    USE_TASK_PWM,           USE_TASK_DIGITAL_OUTPUT, USE_TASK_PWM,
    USE_TASK_DIGITAL_OUTPUT};

int task_list_num = sizeof(task_list) / sizeof(TaskType_t);

int current_task_index = -1;

int increment_current_task_index() {
  current_task_index++;
  if (current_task_index >= task_list_num) {
    current_task_index = 0;
  }
  return current_task_index;
}

void log_current_task() {
  if (task_list[current_task_index] == USE_TASK_I2C) {
    alogln("[LOG]: Current task I2C");
  } else if (task_list[current_task_index] == USE_TASK_PWM) {
    alogln("[LOG]: Current task PWM");
  } else if (task_list[current_task_index] == USE_TASK_DIGITAL_OUTPUT) {
    alogln("[LOG]: Current task DIGITAL OUTPUT");
  } else if (task_list[current_task_index] == USE_TASK_DIGITAL_INPUT) {
    alogln("[LOG]: Current task DIGITAL INPUT");
  } else {
    alogln("[ERR]: Current task UNKNOWN !!!!");
  }
}

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

  if (task_list[current_task_index] == USE_TASK_I2C) {

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

void digital_input_task() {
  static long start = millis();

  if (task_list[current_task_index] == USE_TASK_DIGITAL_INPUT) {
    if (millis() - start > DIGITAL_INPUT_TASK_ms) {
      start = millis();
      alogln("[LOG]: Reading digital pins");

      pinMode(ARDUINO_WIRE_SCL_PIN, INPUT);
      pinMode(ARDUINO_WIRE_SDA_PIN, INPUT);

      auto status = digitalRead(ARDUINO_WIRE_SDA_PIN);
      auto status1 = digitalRead(ARDUINO_WIRE_SCL_PIN);

      alogln("[LOG]: pin SDA (9) is %s", (status == HIGH) ? "HIGH" : "LOW");
      alogln("[LOG]: pin SCL (8) is %s", (status1 == HIGH) ? "HIGH" : "LOW");
    }
  }
}

void digital_output_task() {
  if (task_list[current_task_index] == USE_TASK_DIGITAL_OUTPUT) {
    static long start = millis();
    static bool status = true;
    if (millis() - start > DIGITAL_OUTPUT_TASk_ms) {
      start = millis();
      alogln("[LOG]: Setting digital pins");

      pinMode(ARDUINO_WIRE_SCL_PIN, OUTPUT);
      pinMode(ARDUINO_WIRE_SDA_PIN, OUTPUT);

      if (status) {
        status = false;
        alogln("[LOG]: setting pin SDA (9) HIGH");
        alogln("[LOG]: setting pin SCL (8) HIGH");
        digitalWrite(ARDUINO_WIRE_SCL_PIN, HIGH);
        digitalWrite(ARDUINO_WIRE_SDA_PIN, HIGH);
      } else {
        status = true;
        alogln("[LOG]: setting pin SDA (9) LOW");
        alogln("[LOG]: setting pin SCL (8) LOW");
        digitalWrite(ARDUINO_WIRE_SCL_PIN, LOW);
        digitalWrite(ARDUINO_WIRE_SDA_PIN, LOW);
      }
    }
  }
}

/* ______________________________________ Activate PWM on the same pin of I2C
 */
void pwm_task() {
  static long start = millis();

  if (task_list[current_task_index] == USE_TASK_PWM) {
    if (i2c_initialized) {
      alogln("[LOG]: Calling wire.end()");
      WIRE.end();
      i2c_initialized = false;
    }

    if (millis() - start > I2C_PWM_TASK_ms) {
      start = millis();
      alogln("[LOG]: Activating PWM");
      analogWrite(ARDUINO_WIRE_SDA_PIN, 50);
      analogWrite(ARDUINO_WIRE_SCL_PIN, 100);
    }
  }
}

/* activate either one of the 2 other tasks i2c or pwm with a certain period
 */
void switch_task() {
  static long start = millis();

  if (millis() - start > SWITCHING_TASK_ms) {
    start = millis();
    increment_current_task_index();
    log_current_task();
    /* this forces i2c task to re initialize I2C always */
    i2c_initialized = false;
  }
}

/* ____________________________________________________________________loop()
 */
void loop() {
  switch_task();
  i2c_task();
  pwm_task();
  digital_output_task();
  digital_input_task();
}
