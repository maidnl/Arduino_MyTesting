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
#include "ArduinoRS485.h"

#define LED_BUILTIN D4

#define SLEEP_TIME_MS 1000

LOG_MODULE_REGISTER(test_rs_485, LOG_LEVEL_INF);

void blink_led() {
	static bool st = true;
	if (st) {
		st = false;
		digitalWrite(LED_BUILTIN, LOW);
	} else {
		st = true;
		digitalWrite(LED_BUILTIN, HIGH);
	}
}

int counter = 0;

void rs_485_tx() {
	printk("A\n");
	RS485.beginTransmission();
	printk("B\n");
	RS485.print("hello ");
	printk("C\n");
	RS485.println(counter);
	printk("D\n");
	RS485.endTransmission();
	printk("E\n");

	counter++;
}

void setup() {
}

void loop() {
	blink_led();
	rs_485_tx();
	delay(SLEEP_TIME_MS);
}
