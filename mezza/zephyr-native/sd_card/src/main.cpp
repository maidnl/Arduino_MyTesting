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
#include <zephyr/fs/fs.h>
#include <ff.h>
#include <Arduino.h>

#define LED_BUILTIN D4

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

LOG_MODULE_REGISTER(sd_test, LOG_LEVEL_INF);

static FATFS fat_fs;
/* Mount point mapping to the SD disk area */
static struct fs_mount_t mp = {
	.type = FS_FATFS,
	.mnt_point = "/SD:",
	.fs_data = &fat_fs,
};

void setup() {
	// Serial.begin(115200);
	pinMode(D0, INPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(D12, OUTPUT);
	pinMode(D13, OUTPUT);
	/* Force SPI4 to use the PLL3 clock source directly via hardware register mapping */
	/* RCC base address is 0x44020C00, CCIPR3 offset is 0xE0 */
	struct fs_file_t file;
	const char *test_filename = "/SD:/test1.txt";
	const char *test_data = "Altra prova per scrivere su SD card!!\n";
	char read_buffer[64] = {0};
	int rc;

	/* 1. Initialize Disk & Validate Card Detect Pin */
	static const char *disk_pdrv = "SD";

	printk("Checking for SD Card. Please insert the card if you haven't already...");

	/* Loop until the cd-gpios pin detects the card and initialization succeeds */
	while (true) {
		rc = disk_access_init(disk_pdrv);
		if (rc == 0) {
			printk("Card Detect Pin Triggered! SD card initialized successfully.\n");
			break; /* Exit loop and proceed to mount */
		}

		printk("Card not detected (rc=%d). Waiting...\n", rc);
		k_msleep(2000); /* Wait 2 seconds before checking the pin again */
	}
	printk("Disk initialized successfully.\n");

	/* 2. Mount File System */
	rc = fs_mount(&mp);
	if (rc != 0) {
		printk("Error mounting SD card (rc=%d). Is it formatted as FAT?", rc);
		return;
	}
	printk("SD card mounted successfully at %s", mp.mnt_point);

	/* 3. Write Test */
	fs_file_t_init(&file);
	rc = fs_open(&file, test_filename, FS_O_CREATE | FS_O_WRITE);
	if (rc < 0) {
		printk("Failed to open file for writing (rc=%d)", rc);
		goto unmount;
	}

	rc = fs_write(&file, test_data, strlen(test_data));
	if (rc < 0) {
		printk("Failed to write to file (rc=%d)", rc);
	} else {
		printk("Wrote %d bytes to %s", rc, test_filename);
	}
	fs_close(&file);

	/* 4. Read Test */
	rc = fs_open(&file, test_filename, FS_O_READ);
	if (rc < 0) {
		printk("Failed to open file for reading (rc=%d)", rc);
		goto unmount;
	}

	rc = fs_read(&file, read_buffer, sizeof(read_buffer) - 1);
	if (rc < 0) {
		printk("Failed to read from file (rc=%d)", rc);
	} else {
		printk("Read %d bytes: '%s'", rc, read_buffer);
	}
	fs_close(&file);

unmount:
	/* 5. Cleanup */
	rc = fs_unmount(&mp);
	if (rc != 0) {
		printk("Error unmounting SD card (rc=%d)", rc);
	} else {
		printk("SD card unmounted successfully. Test Complete.");
	}
}

void loop() {
	// Serial.println("non premuto");
	digitalWrite(LED_BUILTIN, LOW);
	// digitalWrite(D13, LOW);
	delay(1000);

	// Serial.println("premuto");
	digitalWrite(LED_BUILTIN, HIGH);
	// digitalWrite(D13, HIGH);
	delay(1000);
}
