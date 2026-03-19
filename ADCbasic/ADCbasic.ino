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
  int v = analogRead(A0);
  alogln("Analog value: %i", v);
  delay(1000);
}
