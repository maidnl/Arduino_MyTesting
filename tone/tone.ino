/* -------------------------------------------------------------------------- */
/* FILE NAME:   tone.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       maidnl74@gmail.com
   DATE:        20260417
   DESCRIPTION:
   LICENSE:     Copyright (c) 2026 Daniele Aimo
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include "pitches.h"

// notes in the melody:
int melody[] = {

    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {

    4, 8, 8, 4, 4, 4, 4, 4};

void play();

/* ___________________________________________________________________setup() */
void setup() {

  Serial.begin(115200);
  while (!Serial) {
  }

  Serial.println("setup");
  // iterate over the notes of the melody:
  play();
}

void play() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.

    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.

    int noteDuration = 1000 / noteDurations[thisNote];

    tone(3, melody[thisNote], noteDuration);
    tone(4, melody[thisNote], noteDuration);
    tone(5, melody[thisNote], noteDuration);
    tone(6, melody[thisNote], noteDuration);
    tone(7, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.

    // the note's duration + 30% seems to work well:

    int pauseBetweenNotes = noteDuration * 1.30;


    delay(pauseBetweenNotes);
    

    // stop the tone playing:

    noTone(3);
    noTone(4);
    noTone(5);
    noTone(6);
    noTone(7);
  }
}

/* ____________________________________________________________________loop() */
void loop() {
  delay(2000);
  //play();
  Serial.println("+++");
  // no need to repeat the melody.
}
