#include "leds.h"

#define BTN D0
#define LED D1

#define REPETITIONS 10
#define DELAY_ms 500

typedef enum { LED1 = D1, LED2 = D4, LED3 = D7 } LED_t;

#define BLU 0
#define GREEN 1
#define RED 2
#define LOFF 4

void led_off(LED_t l) {
  digitalWrite(l + BLU, LOW);
  digitalWrite(l + GREEN, LOW);
  digitalWrite(l + RED, LOW);
}

void led_blu(LED_t l) {
  digitalWrite(l + BLU, HIGH);
  digitalWrite(l + GREEN, LOW);
  digitalWrite(l + RED, LOW);
}

void led_green(LED_t l) {
  digitalWrite(l + BLU, LOW);
  digitalWrite(l + GREEN, HIGH);
  digitalWrite(l + RED, LOW);
}

void led_red(LED_t l) {
  digitalWrite(l + BLU, LOW);
  digitalWrite(l + GREEN, LOW);
  digitalWrite(l + RED, HIGH);
}

void led_control(LED_t l, int color) {
  if (color == BLU) {
    led_blu(l);
  } else if (color == GREEN) {
    led_green(l);
  } else if (color == RED) {
    led_red(l);
  } else {
    led_off(l);
  }
}

void mezza_test_leds() {
  static unsigned long t = 0;
  if (millis() - t > 500) {
    t = millis();

    static int color = BLU;
    static LED_t led = LED1;
    static bool st = true;

    if (color == LOFF) {
      led_off(led);

      if (led == LED1) {
        led = LED2;
      } else if (led == LED2) {
        led = LED3;
      } else if (led == LED3) {
        led = LED1;
      }

      color = BLU;
    }
    led_control(led, color);

    color = color + 1;

    if (st) {
      st = false;
      digitalWrite(D10, HIGH);
      digitalWrite(D11, LOW);
    } else {
      st = true;
      digitalWrite(D10, LOW);
      digitalWrite(D11, HIGH);
    }
  }
}

void mezza_test_init_leds() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);
  pinMode(D11, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  digitalWrite(D9, LOW);
}
