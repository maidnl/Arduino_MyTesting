#ifndef MEZZA_TEST_SDCARD_TEST
#define MEZZA_TEST_SDCARD_TEST

#define SD_DELAY_MS 500
#define SD_CARD_WAIT_FOR_CARD_DETECTION_ms 500
#define SD_WRITE_DIM 64
#define SD_READ_DIM 64

#include <Arduino.h>
#include <ff.h>
#include <stm32_ll_rcc.h>
#include <zephyr/device.h>
#include <zephyr/drivers/clock_control/stm32_clock_control.h>
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/disk_access.h>

void mezza_test_sdcard();
void mezza_test_init_sdcard();

#endif
