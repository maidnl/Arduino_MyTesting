#include "atest_serial.h"
#include "Arduino.h"

/* _____get single (first char) typed on serial (return - if nothing is typed)*/
char atestGetCharCmd() {
  char rv = 255;
  if (!Serial.available()) {
    return NO_CHAR_CMD;
  }

  bool read = true;

  while (Serial.available()) {
    int num = Serial.read();
    Serial.println(num);
    if (read) {
      read = false;
      rv = (char)num;
    }
  }
  return rv;
}

bool atestChangeTask() {
  char cmd = atestGetCharCmd();
  if (cmd == NEXT_TASK_CMD) {
    return true;
  }
  return false;
}
