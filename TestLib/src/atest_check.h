#include "atest_config.h"
#include "atest_log.h"

#ifndef ATEST_CHECK_H
#define ATEST_CHECK_H

#define AT_PASSED 1
#define AT_FAILED 0

template <class T> int test_check_buffer(T *obtained, T *expected, size_t dim) {
  for (size_t i = 0; i < dim; i++) {
    if (*(expected + i) != *(obtained + i)) {
      return 0;
    }
  }
  return 1;
}

template <class T>
int test_check_i2c_answer(T *obtained, T *expected, size_t dim) {
  int rv = AT_FAILED;
  if (dim > AT_WIRE_RX_BUFFER_DIM) {
    dim = AT_WIRE_RX_BUFFER_DIM;
  }

  alog("[LOG]: I2C rx answer: ");
  for (int i = 0; i < dim; i++) {
    Serial.print((char)*(obtained + i));
  }
  Serial.println();

  alog("[LOG]: I2C  expected: ");
  for (int i = 0; i < dim; i++) {
    Serial.print((char)*(expected + i));
  }
  Serial.println();

  if (test_check_buffer<T>(obtained, expected, dim)) {
    alogln("[RES]: PASSED");
    rv = AT_PASSED;
  } else {
    alogln("[RES]: FAILED");
  }
  memset(obtained, 0x3F, dim);
  /*
  for (int i = 0; i < dim; i++) {
    Serial.print((char)*(obtained + i));
  }
  Serial.println();
  */
  return rv;
}

#endif
