#include "compliance_rs485.h"
#include <cstdint>

#define BAUD_RATE 14745600
// #define BAUD_RATE 7372800
// #define BAUD_RATE 3686400
// #define BAUD_RATE 1843200
// #define BAUD_RATE 921600

#include "crc.h"

uint8_t tx_buffer[20];
uint8_t counter[3];
static uint8_t address = 1;

void init_rs485() {
  if (Serial) {
    Serial.println("### Init RS485:");
  }
  RS485.begin(BAUD_RATE);
}

void rs485_task() {
#ifdef LOG_RS485_TASK
  if (Serial) {
    Serial.println("[TASK]: RS485:");
  }
#endif
  if (RS485.available()) {
    while (RS485.available()) {
      RS485.read();
    }
  }
}
