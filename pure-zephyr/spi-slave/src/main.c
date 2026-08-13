#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/usb/usb_device.h>

LOG_MODULE_REGISTER(spi_slave_async, LOG_LEVEL_INF);
/**
 * @file main.c
 * @brief USB Serial Hello World for Arduino Giga
 */
/* 1. Get the SPI device from Devicetree */
#define SPI_SLAVE_NODE DT_NODELABEL(spi5)
static const struct device *spi_dev = DEVICE_DT_GET(SPI_SLAVE_NODE);

/* Buffers for communication */
__aligned(32) static uint8_t rx_buffer[32];
__aligned(32) static uint8_t tx_buffer[32] = "Hello Master!";

/* Signal to notify the main thread when the transfer is done */
static struct k_poll_signal async_sig = K_POLL_SIGNAL_INITIALIZER(async_sig);

void spi_async_callback(const struct device *dev, int result, void *data) {
  if (result == 0) {
    LOG_INF("SPI transfer completed successfully.");
  } else {
    LOG_ERR("SPI transfer failed with error: %d", result);
  }

  /* Raise the signal to wake up the main thread */
  k_poll_signal_raise(&async_sig, result);
}

int main(void) {
  const struct device *dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
  uint32_t dtr = 0;

  /* 1. Initialize the USB Stack */
  if (usb_enable(NULL)) {
    return 0;
  }
  if (!device_is_ready(spi_dev)) {
    LOG_ERR("SPI slave device not ready");
    return 0;
  }
  /* 2. Wait for the user to connect via Serial Monitor
   * This prevents the code from printing before you open the terminal.
   */
  while (!dtr) {
    uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
    k_sleep(K_MSEC(100));
  }

  struct spi_config config = {
      .frequency = 1000000,
      .operation = SPI_OP_MODE_SLAVE | SPI_WORD_SET(8),
      .slave = 0,
  };

  struct spi_buf tx_buf = {.buf = tx_buffer, .len = sizeof(tx_buffer)};
  struct spi_buf_set tx_bufs = {.buffers = &tx_buf, .count = 1};

  struct spi_buf rx_buf = {.buf = rx_buffer, .len = sizeof(rx_buffer)};
  struct spi_buf_set rx_bufs = {.buffers = &rx_buf, .count = 1};

  LOG_INF("SPI Slave initialized. Starting first async read...");
  int ret = 0;

  k_sleep(K_SECONDS(5));
  k_poll_signal_reset(&async_sig);
  /* 1. Kick off the FIRST asynchronous transceive before the loop */
  ret = spi_transceive_cb(spi_dev, &config, &tx_bufs, &rx_bufs,
                          spi_async_callback, NULL);
  if (ret < 0) {
    LOG_ERR("spi_transceive_cb failed: %d", ret);
    return 0;
  }
  /* 3. Infinite loop printing every 1000ms */
  while (1) {

    /* 5. Wait for the signal (non-blocking for the CPU, but blocking
     *    this thread) */
    struct k_poll_event events[] = {
        K_POLL_EVENT_INITIALIZER(K_POLL_TYPE_SIGNAL, K_POLL_MODE_NOTIFY_ONLY,
                                 &async_sig),
    };
    /* 2. Check the signal without blocking (K_NO_WAIT) */
    int poll_ret = k_poll(events, ARRAY_SIZE(events), K_NO_WAIT);

    if (poll_ret == 0) {
      /* 3. poll_ret == 0 means the signal was raised! Transfer is
       *    complete. */
      LOG_HEXDUMP_INF(rx_buffer, sizeof(rx_buffer), "Received from Master:");

      /* Reset the signal so we can use it again */
      k_poll_signal_reset(&async_sig);

      /* Set up the NEXT asynchronous transfer to listen again */
      ret = spi_transceive_cb(spi_dev, &config, &tx_bufs, &rx_bufs,
                              spi_async_callback, NULL);
      if (ret < 0) {
        LOG_ERR("Failed to restart spi_transceive_cb: %d", ret);
      }
    }
    printk("Main loop running... \n");
    k_sleep(K_SECONDS(1));
  }

  return 0;
}
