#ifndef A_TEST_WIRE_H
#define A_TEST_WIRE_H

#include "atest_log.h"
#include <cstdint>

template <class T> int i2c_init(T &wire, uint8_t add = 0, bool log = true) {
  if (add == 0) {
    if (log) {
      alogln("[LOG]: calling wire begin as I2C master");
    }
    wire.begin();
  } else {
    if (log) {
      alogln("[LOG]: calling wire begin as I2C slave with address %i", add);
    }
    wire.begin(add);
  }
  return 0;
}

/**
 * @brief Send an I2C message as Master
 *
 * @param wire: a reference to the Wire instance to be used
 * @param add: the address of the slave to be addressed
 * @param buf: a pointer to an uint8_t buffer containing the data to be sent
 * @param rx: a buffer to put the slave answer into ( if nullptr no answer is
 * expected
 * @param rx_dim: dimension of the rx buffer (the number of bytes expected to be
 * reading -> the buffer must have at least the same dimension)
 * @param log: a boolean specifying if the function must output the log or not
 */

template <class T>
int i2c_send_master(T &wire, uint8_t add, uint8_t *buf, uint32_t dim,
                    uint8_t *rx, uint32_t rx_dim, uint32_t timeout_ms,
                    bool log = true) {
  /* LOG data to be sent */
  if (log) {
    alog("[LOG]: I2C Master send data: ");
    for (uint32_t i = 0; i < dim; i++) {
      alog("0x%02X ", buf[i]);
    }
    alogln("");
  }

  /* TRANSMIT */
  wire.beginTransmission(add);

  for (uint32_t i = 0; i < dim; i++) {
    wire.write(*(buf + i));
  }

  int res = wire.endTransmission();

  /* LOG result of transmission */
  if (log) {
    alogln("[RES]: I2C send result %i", res);
  }

  /* RECEIVE if necessary */
  if (rx != nullptr && rx_dim != 0 && res == 0) {
    uint32_t rindex = 0;
    memset(rx, 0x00, rx_dim);

    if (log)
      alogln("[LOG]: answer expected from slave");

    wire.requestFrom(add, rx_dim);

    unsigned long int start = millis();
    while (millis() - start < timeout_ms && rindex < rx_dim) {
      while (wire.available()) {
        rx[rindex++] = wire.read();
      }
    }
    if (log) {
      alog("[LOG]: I2C Master received data: ");
      for (uint32_t i = 0; i < rx_dim; i++) {
        alog("0x%02X ", *(rx + i));
      }
      alogln("");
    }
  }
  return 0;
}
#endif
