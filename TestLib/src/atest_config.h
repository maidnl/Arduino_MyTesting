#ifndef ATEST_CONFIG
#define ATEST_CONFIG

#include "Arduino.h"
#include "Wire.h"
#define ZEPHYR_TEST

#ifdef ZEPHYR_TEST
typedef arduino::ZephyrI2C Wire_t;
#else

#if defined(ARDUINO_NANO33BLE)
typedef arduino::MbedI2C;
#elif defined(ARDUINO_GIGA)
typedef arduino::MbedI2C;
#endif

#endif

#define AT_WIRE_RX_BUFFER_DIM 256
#define AT_WIRE_TX_BUFFER_DIM 256

#endif
