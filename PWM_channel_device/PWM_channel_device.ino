
/* -------------------------------------------------------------------------- */
/* FILE NAME:   pwm.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:
   DESCRIPTION: Test sketch for I2C master device
   NOTES:       To be used with slave.ino                                     */
/* -------------------------------------------------------------------------- */
#include "Arduino.h"
#include "atestlib.h"

/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  alogln("[LOG]: DIGITAL OUTPUT HIGH");

  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);

  delay(1000);

  alogln("[LOG]: PWM activated!");
  // analogWrite(9, 50);
  analogWrite(8, 100);
}

/* ____________________________________________________________________loop() */
void loop() { delay(1000); }
