#include "atest_task.h"

static int current_task_index = -1;

extern TaskType_t task_list[];

int getTaskIndex() { return current_task_index; }

int increment_current_task_index(int max_task_index) {
  if (current_task_index == -1) {
    current_task_index = 0;
  }

  current_task_index++;
  if (current_task_index >= max_task_index) {

    current_task_index = 0;
  }
  return current_task_index;
}

void log_current_task() {
  if (task_list[current_task_index] == USE_TASK_I2C) {
    alogln("[LOG]: Current task I2C");
  } else if (task_list[current_task_index] == USE_TASK_PWM) {
    alogln("[LOG]: Current task PWM");
  } else if (task_list[current_task_index] == USE_TASK_DIGITAL_OUTPUT) {
    alogln("[LOG]: Current task DIGITAL OUTPUT");
  } else if (task_list[current_task_index] == USE_TASK_DIGITAL_INPUT) {
    alogln("[LOG]: Current task DIGITAL INPUT");
  } else if (task_list[current_task_index] == USE_TASK_ANALOG_INPUT) {
    alogln("[LOG]: Current task DIGITAL INPUT");
  } else {
    alogln("[ERR]: Current task UNKNOWN !!!!");
  }
}

void switch_task(int mt) {
  static long start = millis();

  if (millis() - start > SWITCHING_TASK_ms) {
    start = millis();
    increment_current_task_index(mt);
    log_current_task();
  }
}

void switch_task_manually(int mt) {
  if (atestChangeTask()) {
    increment_current_task_index(mt);
    log_current_task();
  }
}

bool execute_task(TaskType_t tt, unsigned long &time, unsigned long period) {
  if (current_task_index == -1) {
    current_task_index = 0;
  }
  unsigned long ct = millis();
  if (task_list[getTaskIndex()] == tt && (ct - time > period)) {
    time = ct;
    return true;
  }
  return false;
}
