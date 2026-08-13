#include "zephyr/sys/printk.h"
#include "zephyr/sys/time_units.h"
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>
LOG_MODULE_REGISTER(spi_slave_thread, LOG_LEVEL_INF);

/* 1. Point to your spi5 device */
#define SPI_SLAVE_NODE DT_NODELABEL(spi5)
static const struct device *spi_dev = DEVICE_DT_GET(SPI_SLAVE_NODE);

/* Buffers for communication */
static uint8_t rx_buffer[32] __aligned(32);
static uint8_t tx_buffer[32] __aligned(32) = "Hello Master!";

/* Thread configuration parameters */
#define SPI_THREAD_STACK_SIZE 1024
#define SPI_THREAD_PRIORITY 5

void spi_slave_entry_point(void *p1, void *p2, void *p3) {
  if (!device_is_ready(spi_dev)) {
    LOG_ERR("SPI slave device not ready in background thread");
    return;
  }

  struct spi_config config = {
      .frequency = 1000000,
      .operation = SPI_OP_MODE_SLAVE | SPI_WORD_SET(8),
      .slave = 0,
  };

  struct spi_buf tx_buf = {.buf = tx_buffer, .len = 16};
  struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};

  struct spi_buf rx_buf = {.buf = rx_buffer, .len = 16};
  struct spi_buf_set rx_bufs = {.buffers = &rx_buf, .count = 1};

  LOG_INF("Background Thread: SPI Slave waiting for Master...");
  while (1) {
    memset(rx_buffer, 0, sizeof(rx_buffer));
    int ret = spi_transceive(spi_dev, &config, &tx_bufs, &rx_bufs);

    if (ret >= 0) {
      LOG_INF("Success! Received %d bytes.", ret);
      /* Transfer is complete! Process the data. */
      LOG_HEXDUMP_INF(rx_buffer, sizeof(rx_buffer),
                      "Received from Arduino Giga:");

    } else {
      LOG_ERR("SPI Error: %d. Resetting driver...", ret);
      spi_release(spi_dev, &config);
      k_msleep(50);
    }
  }
}

/* 3. Define and automatically start the background thread */
K_THREAD_DEFINE(spi_slave_tid, SPI_THREAD_STACK_SIZE, spi_slave_entry_point,
                NULL, NULL, NULL, SPI_THREAD_PRIORITY, 0, SYS_FOREVER_MS);

/* 4. Your Main Application */
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
  int count = 0;
  while (!dtr) {
    uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
    count++;
    k_sleep(K_MSEC(100));
  }
  LOG_INF("Main Thread: Running free! count = %i", count);
  LOG_INF("Starting SPI background thread...");
  k_thread_start(spi_slave_tid);
  while (1) {
    /* * Because the SPI work is happening in the background thread,
     * this main loop is completely unblocked.
     */
    LOG_INF("Main Thread: Doing other work...");

    /* Simulate doing some other work */
    k_msleep(2000);
  }
  return 0;
}
