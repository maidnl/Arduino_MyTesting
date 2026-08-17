#include "user_button.h"

#define BTN D0
#define LED D1

#define REPETITIONS 10
#define DELAY_ms 500

void mezza_test_userbutton() {
  if (digitalRead(BTN) == HIGH) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

void mezza_test_init_userbutton() {
  pinMode(BTN, INPUT);
  pinMode(LED, OUTPUT);
}
