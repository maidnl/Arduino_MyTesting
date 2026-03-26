#ifndef ATEST_TASK_H
#define ATEST_TASK_H
#include "Arduino.h"
#include "atest_log.h"
#include "atest_serial.h"

#define SWITCHING_TASK_ms 10000

typedef enum {
  USE_TASK_I2C,
  USE_TASK_PWM,
  USE_TASK_DIGITAL_INPUT,
  USE_TASK_DIGITAL_OUTPUT,
  USE_TASK_ANALOG_INPUT
} TaskType_t;

void switch_task(int mt);

void switch_task_manually(int mt);

bool execute_task(TaskType_t tt, unsigned long &time, unsigned long period);
#endif
