/* -------------------------------------------------------------------------- */
/* FILE NAME:   mezzaTest.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       maidnl74@gmail.com
   DATE:        20260813
   DESCRIPTION:
   LICENSE:     Copyright (c) 2026 Daniele Aimo
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "leds.h"
#include "relays.h"
#include "sdcard.h"
#include "user_button.h"

/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  Serial.println("---- TEST SKETCH FOR MEZZA ----");
  // mezza_test_init_userbutton();
  // mezza_test_init_leds();
  mezza_test_init_sdcard();
  // mezza_test_relays();
}

/* ____________________________________________________________________loop()
 */
void loop() {
  // mezza_test_userbutton();
  // mezza_test_leds();
  delay(100);
}
