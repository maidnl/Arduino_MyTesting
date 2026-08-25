#include "compliance_can.h"

static uint32_t const CAN_ID = 0x123;

void init_can() {
#ifdef LOG_CAN_TASK
  if (Serial)
    Serial.println("### Init CAN:");
#endif
  if (!CAN.beginFD(CanBitRate::BR_500k, 1000000)) {
#ifdef LOG_CAN_TASK
    if (Serial)
      Serial.println("CAN.beginFD(...) failed.");
  }
#endif

#ifdef LOG_CAN_TASK
  if (Serial)
    Serial.println("### Init CAN1:");
#endif
  if (!CAN1.beginFD(CanBitRate::BR_500k, 1000000)) {
#ifdef LOG_CAN_TASK
    if (Serial)
      Serial.println("CAN.beginFD(...) failed.");
#endif
  }
}

void can_task() {
#ifdef LOG_CAN_TASK
  if (Serial)
    Serial.println("[TASK]: CAN TASK");
#endif

  if (CAN.available()) {
    CanFDMsg const msg = CAN.readFD();
  }
  if (CAN1.available()) {
    CanFDMsg const msg = CAN.readFD();
  }
}
