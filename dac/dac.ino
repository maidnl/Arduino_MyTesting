/* -------------------------------------------------------------------------- */
/* FILE NAME:   dac.ino
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
#include <array>
#define SAMPLES_FOR_PREDEFINED_WAVE 24

static constexpr std::array<uint8_t, SAMPLES_FOR_PREDEFINED_WAVE> getSine() {
  std::array<uint8_t, SAMPLES_FOR_PREDEFINED_WAVE> a{};

  float step = (2 * 3.1415) / SAMPLES_FOR_PREDEFINED_WAVE;

  for (size_t i = 0; i < a.size(); ++i) {
    float sample = (1 + sin((float)i * step)) * (float)255 / 2.0;
    a[i] = (uint8_t)sample;
  }
  return a;
}

static const std::array<uint8_t, SAMPLES_FOR_PREDEFINED_WAVE> _sinewave =
    getSine();
/* ___________________________________________________________________setup() */
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
}

/* ____________________________________________________________________loop() */
void loop() {
  static int i = 0;

  analogWrite(DAC0, _sinewave[i]);
  analogWrite(DAC1, _sinewave[i]);

  i++;
  if (i >= SAMPLES_FOR_PREDEFINED_WAVE) {
    i = 0;
  }
  delay(1);
}
