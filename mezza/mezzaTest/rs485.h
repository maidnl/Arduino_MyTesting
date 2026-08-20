#ifndef MEZZA_TEST_RS485_TEST
#define MEZZA_TEST_RS485_TEST

#include "Arduino.h"
#include <ArduinoModbus.h>
#include <ArduinoRS485.h>

// ==========================================
// CONFIGURATION
// ==========================================

// Change this for each of the 4 devices before uploading:
// 0 = Master (Transmitter)
// 1 = Slave 1(Receiver)
// 2 = Slave 2 (Receiver)
// 3 = Slave 3 (Receiver)
#define DEVICE_ADDRESS 0

// Configurable Baud Rate
#define MODBUS_BAUD 9600

#define TASK_MODBUS_ms 1000
// the master tx data to 3 different slaves
// this is the delay between each tx
#define DELAY_TX_ms 50

void mezza_test_rs485();
void mezza_test_init_rs485();

#endif
