#include "compliance_wifi.h"

static int numSsid = 0;

void init_wifi() {}

void wifi_task() {
#ifdef LOG_WIFI_TASK
  if (Serial)
    Serial.println("[TASK]: WIFI TASK: Scanning available networks...");
#endif
  numSsid = WiFi.scanNetworks();
  if (numSsid <= 0) {
#ifdef LOG_WIFI_TASK
    if (Serial)
      Serial.println("No networks found, trying again...");
#endif
  } else {
#ifdef LOG_WIFI_TASK
    if (Serial) {
      Serial.print("Found ");
      Serial.print(numSsid);
      Serial.println(" networks");
    }
#endif
  }
}
