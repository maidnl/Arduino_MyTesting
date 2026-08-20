/*
 * SPDX-License-Identifier: Apache-2.0
 */

/* Blink inbuilt LED example */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/storage/disk_access.h>
#include <zephyr/drivers/clock_control/stm32_clock_control.h>
#include <stm32_ll_rcc.h>
#include <zephyr/logging/log.h>
#include <Arduino.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

LOG_MODULE_REGISTER(led_test, LOG_LEVEL_INF);

void setup() {
	// Serial.begin(115200);
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

typedef enum {
	LED1,
	LED2,
	LED3
} LED_t;

typedef enum {
	BLU,
	GREEN,
	RED
} COLOR_t;

static int first_pin = D1;
static LED_t led = LED1;
static COLOR_t color = BLU;

bool led_control() {
	bool rv = false;
	if (color == BLU) {
		digitalWrite(first_pin, HIGH);
		digitalWrite(first_pin + 1, LOW);
		digitalWrite(first_pin + 2, LOW);
		color = GREEN;
	} else if (color == GREEN) {
		digitalWrite(first_pin, LOW);
		digitalWrite(first_pin + 1, HIGH);
		digitalWrite(first_pin + 2, LOW);
		color = RED;

	} else if (color == RED) {
		digitalWrite(first_pin, LOW);
		digitalWrite(first_pin + 1, LOW);
		digitalWrite(first_pin + 2, HIGH);
		color = BLU;
		rv = true;
	}
	delay(500);
	return rv;
}

static bool st = true;

void loop() {

	if (led == LED1) {
		first_pin = D1;
		if (led_control()) {
			digitalWrite(first_pin, LOW);
			digitalWrite(first_pin + 1, LOW);
			digitalWrite(first_pin + 2, LOW);

			led = LED2;
		}

	} else if (led == LED2) {
		first_pin = D4;
		if (led_control()) {
			digitalWrite(first_pin, LOW);
			digitalWrite(first_pin + 1, LOW);
			digitalWrite(first_pin + 2, LOW);
			led = LED3;
		}

	} else if (led == LED3) {
		first_pin = D7;
		if (led_control()) {
			digitalWrite(first_pin, LOW);
			digitalWrite(first_pin + 1, LOW);
			digitalWrite(first_pin + 2, LOW);
			led = LED1;
		}
	}

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
