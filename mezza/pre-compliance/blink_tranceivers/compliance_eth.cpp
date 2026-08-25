#include "compliance_eth.h"

char server[] = "www.google.com";

IPAddress ip(192, 168, 2, 177);
IPAddress myDns(8, 8, 8, 8);

ZephyrClient client;

unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = false;

static bool eth_initialized = false;

void init_eth() {
#ifdef LOG_ETH_TASK
  if (Serial) {
    Serial.print("#### INIT ETH");
  }
#endif
  if (!eth_initialized) {
    if (Ethernet.linkStatus() == LinkON) {
      if (Ethernet.begin() == 0) {
        if (Ethernet.begin(ip, myDns)) {
          eth_initialized = true;
          delay(1000);
        }
      } else {
        eth_initialized = true;
        delay(1000);
#ifdef LOG_ETH_TASK
        if (Serial) {
          Serial.print("  DHCP assigned IP ");
          Serial.println(Ethernet.localIP());
        }
#endif
      }
    }
  }
}

void eth_task() {
  init_eth();
  bool connected = false;
  if (eth_initialized) {

#ifdef LOG_ETH_TASK
    if (Serial) {
      Serial.print("connecting to ");
      Serial.print(server);
      Serial.println("...");
    }
#endif

    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
#ifdef LOG_ETH_TASK
      if (Serial) {
        Serial.print("connected to ");
        Serial.println(client.remoteIP());
        // Make a HTTP request:
        client.println("GET /search?q=arduino HTTP/1.1");
        client.println("Host: www.google.com");
        client.println("Connection: close");
        client.println();
        connected = true;
      }
#endif
    } else {
      // if you didn't get a connection to the server:
#ifdef LOG_ETH_TASK
      if (Serial) {
        Serial.println("connection failed");
      }
#endif
    }
    if (connected) {
      int len = client.available();
      if (len > 0) {
        byte buffer[80];
        if (len > 80)
          len = 80;
        client.read(buffer, len);
        if (printWebData) {
#ifdef LOG_ETH_TASK
          if (Serial)
            Serial.write(buffer, len);
#endif
        }
        byteCount = byteCount + len;
      }

      if (!client.connected()) {
#ifdef LOG_ETH_TASK
        if (Serial) {
          Serial.println();
          Serial.println("disconnecting.");
        }
#endif
        client.stop();
      }
    }
  }
}
