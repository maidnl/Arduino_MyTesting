/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/
#include "Wire.h"
// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200);
  while(!Serial){}
  delay(2000);
  Serial.println("+++++++++++++++++++++++++");
  
  // initialize digital pin LED_BUILTIN as an output.
  Wire.begin();

  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);  
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(A4, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
  digitalWrite(A5, HIGH);
  delay(50);
  digitalWrite(A4, LOW);                      // wait for a second
  digitalWrite(A5, LOW);   // change state of the LED by setting the pin to the LOW voltage level
  delay(50);                      // wait for a second
}
