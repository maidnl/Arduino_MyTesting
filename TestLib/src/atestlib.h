#include "atest_check.h"
#include "atest_config.h"
#include "atest_log.h"
#include "atest_task.h"
#include "atest_wire.h"
#include <cstdint>

void atestInit(const char *title) {
  Serial.begin(115200);
  while (!Serial) {
  }
  alogln(title);
}
