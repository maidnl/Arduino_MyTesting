/* -------------------------------------------------------------------------- */
/* FILE NAME:   blink+serial.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       d.aimo@arduino.cc
   DATE:        20260825
   DESCRIPTION:
   LICENSE:     Copyright (c) 2024 Arduino SA
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

void setup() {
  Serial.begin(115200);
  delay(2000);
  if (Serial)
    Serial.println("### Running setup:");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial)
    Serial.println("### Looping: ");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
