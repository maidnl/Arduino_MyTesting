#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>

/**
 * @file main.c
 * @brief USB Serial Hello World for Arduino Giga
 */

int main(void) {
  const struct device *dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
  uint32_t dtr = 0;

  /* 1. Initialize the USB Stack */
  if (usb_enable(NULL)) {
    return 0;
  }

  /* 2. Wait for the user to connect via Serial Monitor
   * This prevents the code from printing before you open the terminal.
   */
  while (!dtr) {
    uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
    k_sleep(K_MSEC(100));
  }

  /* 3. Infinite loop printing every 1000ms */
  while (1) {
    printk("Hello World from Arduino Giga! \n");
    k_sleep(K_SECONDS(1));
  }

  return 0;
}
