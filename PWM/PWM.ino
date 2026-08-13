
/* -------------------------------------------------------------------------- */
/* FILE NAME:   pwm.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:
   DESCRIPTION: Test sketch for I2C master device
   NOTES:       To be used with slave.ino                                     */
/* -------------------------------------------------------------------------- */
#include "Arduino.h"
//#include "atestlib.h"

/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("*** TEST PWM Sketch ***");
  //analogWrite(0, 20);
  //analogWrite(1, 40);
  //analogWrite(2, 20);
  //analogWrite(3, 40);
  analogWrite(4, 150);
  analogWrite(5, 80);
  //analogWrite(6, 100);
  //analogWrite(7, 150);
  //analogWrite(8, 200);
  //analogWrite(9, 40);
  //analogWrite(10, 60);
  /*
  analogWrite(11, 130);
  analogWrite(12, 40);
  analogWrite(13, 60);
  
  analogWrite(A0, 150);
  analogWrite(A1, 200);
  analogWrite(A2, 40);
  analogWrite(A3, 60);
  analogWrite(A4, 130);
  analogWrite(A5, 40);
  analogWrite(A5, 60);
  analogWrite(A7, 60);
  */

}

/* ____________________________________________________________________loop() */
void loop() { delay(1000); 
Serial.println("*** TEST PWM Sketch - loop ***");
}
