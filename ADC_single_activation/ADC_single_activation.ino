
/* -------------------------------------------------------------------------- */
/* FILE NAME:   pwm.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:
   DESCRIPTION: Test sketch for ADC
   Puts just 1 ADC as input and toggle all other pin related to ADC as digital
   output
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */
#include "Arduino.h"
#include "atestlib.h"

/* How test works:
 * - the task_change_pin configure all pin as digital output except the one
 * marked as used_adc (which is incremented every time the task runs)
 *
 * - to activate the task is necessary to type something on the serial (this
 * allows to have time for measurements)
 *
 * - digital outputs are toggled every 50 ms
 *
 * NOTE: first pin to be used as ADC is A1 (last A0)
 * */

#define CHANGE_PIN_ms 500
#define TOGGLE_PINS_ms 50
#define READ_ANALOG_ms 1000

#define ADC_START A0
#define ADC_STOP A7

uint8_t used_adc = ADC_START;

/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  alogln("*** TEST ADC single channel activation Sketch ***");
}

/* _____get single (first char) typed on serial (return - if nothing is typed)*/
char getCharFromSerial() {
  char rv = 255;
  if (!Serial.available()) {
    return '-';
  }

  bool read = true;

  while (Serial.available()) {
    int num = Serial.read();
    if (read) {
      read = false;
      rv = (char)num;
    }
  }
  return rv;
}

/* ______________________________set all pins to digital outputs but used_adc */
void initialize_pins() {
  for (int i = ADC_START; i <= ADC_STOP; i++) {
    if (i == used_adc) {

    } else {
      pinMode(i, OUTPUT);
    }
  }
}

/* ____________________________set all digital outputs to high (but used_adc) */
void pins_high() {
  for (int i = ADC_START; i <= ADC_STOP; i++) {
    if (i == used_adc) {

    } else {
      digitalWrite(i, HIGH);
    }
  }
}

/* ____________________________set all digital outputs to low (but used_adc) */
void pins_low() {
  for (int i = ADC_START; i <= ADC_STOP; i++) {
    if (i == used_adc) {

    } else {
      digitalWrite(i, LOW);
    }
  }
}

/* _________change the pin used for adc and set all others to digital output */
void task_change_pin() {
  static bool first = true;
  static unsigned long start = millis();
  if (((millis() - start) > CHANGE_PIN_ms) || first) {
    char c = getCharFromSerial();
    if (c == '-') {
      if (first == false) {
        return;
      }
    }
    first = false;
    start = millis();
    used_adc++;
    if (used_adc > ADC_STOP) {
      used_adc = ADC_START;
    }
    initialize_pins();
    alogln("[LOG]: new ADC pin is %i", used_adc);
  }
}

/* ________________________toggle pin status using pins_high() and pin_low() */
void task_set_pins() {
  static unsigned long start = millis();
  static bool high = true;
  if (millis() - start > TOGGLE_PINS_ms) {

    start = millis();

    if (high) {
      pins_high();
      high = false;
    } else {
      pins_low();
      high = true;
    }
  }
}

/* ___________read the single pin used_adc and print the value on the serial */
void task_read_analog() {
  static unsigned long start = millis();
  if (millis() - start > READ_ANALOG_ms) {
    start = millis();
    int ar = analogRead(used_adc);
    alogln("[LOG]: voltage read in the ADC pin %i", ar);
  }
}

/* ____________________________________________________________________loop() */
void loop() {
  task_change_pin();
  task_set_pins();
  task_read_analog();
}
