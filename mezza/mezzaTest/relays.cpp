#include "relays.h"

#define RELAY1 D12
#define RELAY2 D13

#define REPETITIONS 10
#define DELAY_ms 500

void test_relay_1() {
  Serial.println("--- TEST RELAY1 ----");

  pinMode(RELAY1, OUTPUT);

  for (int i = 0; i < REPETITIONS; i++) {
    digitalWrite(RELAY1, HIGH);
    delay(DELAY_ms);
    digitalWrite(RELAY1, LOW);
    delay(DELAY_ms);
  }
}

void test_relay_2() {
  Serial.println("--- TEST RELAY2 ----");

  pinMode(RELAY2, OUTPUT);

  for (int i = 0; i < REPETITIONS; i++) {
    digitalWrite(RELAY2, HIGH);
    delay(DELAY_ms);
    digitalWrite(RELAY2, LOW);
    delay(DELAY_ms);
  }
}
void mezza_test_relays() {
  Serial.println("--- TESTING RELAYS ----");
  test_relay_1();
  test_relay_2();
}
