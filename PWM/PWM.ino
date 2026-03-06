
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
  alogln("*** TEST PWM Sketch ***");
  analogWrite(9, 50);
  analogWrite(8, 100);
}

/* ____________________________________________________________________loop() */
void loop() { delay(1000); }
