#ifndef COMPLIANCE_RS485_H
#define COMPLIANCE_RS485_H

#include "ArduinoRS485.h"
#include <Arduino.h>

#define LOG_RS485_TASK

void init_rs485();
void rs485_task();
#endif
