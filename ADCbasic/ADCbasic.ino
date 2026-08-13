/* -------------------------------------------------------------------------- */
/* FILE NAME:   ADCbasic.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       maidnl74@gmail.com
   DATE:        20260319
   DESCRIPTION:
   LICENSE:     Copyright (c) 2026 Daniele Aimo
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

/* TEST "pure" ADC pins */

#include "atestlib.h"
/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  alogln("*** BASIC ADC TEST ***");
}

/* ____________________________________________________________________loop() */
void loop() {
  alogln("");

  int v = analogRead(A0);
  alogln("A0 Analog value: %i", v);

  v = analogRead(A1);
  alogln("A1 Analog value: %i", v);

  v = analogRead(A2);
  alogln("A2 Analog value: %i", v);

  v = analogRead(A3);
  alogln("A3 Analog value: %i", v);

  v = analogRead(A4);
  alogln("A4 Analog value: %i", v);

  v = analogRead(A5);
  alogln("A5 Analog value: %i", v);

  v = analogRead(A6);
  alogln("A6 Analog value: %i", v);

  v = analogRead(A7);
  alogln("A7 Analog value: %i", v);
/*
  v = analogRead(A8);
  alogln("A8 Analog value: %i", v);

  v = analogRead(A9);
  alogln("A9 Analog value: %i", v);

  v = analogRead(A10);
  alogln("A10 Analog value: %i", v);

  v = analogRead(A11);
  alogln("A11 Analog value: %i", v);
  */
  delay(1000);
}
