/* -------------------------------------------------------------------------- */
/* FILE NAME:   wifi-datarate.ino
   AUTHOR:      Daniele Aimo
   EMAIL:       maidnl74@gmail.com
   DATE:        20260824
   DESCRIPTION:
   LICENSE:     Copyright (c) 2026 Daniele Aimo
                his Source Code Form is subject to the terms fo the Mozilla
                Public License (MPL), v 2.0. You can obtain a copy of the MPL
                at http://mozilla.org/MPL/2.0/.
   NOTES:                                                                     */
/* -------------------------------------------------------------------------- */

#include <Arduino.h>
#include <WiFi.h>

//const char *ssid = "diliad-ospite";
//const char *password = "Nmmsfubdf.32";

const char *ssid = "iliadbox-2C4336";
const char *password = "vft75qhvb5q3tc2rqscx6b";

//const char *ssid = "dhowifi";
//const char *password = "nmmsfubdf.32";

const char *serverAddress = "192.168.1.144"; // IP address of your testing PC
const int serverPort = 5001;

WiFiClient client;
uint8_t buffer[1024];

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  // Fill buffer with dummy data
  for (int i = 0; i < sizeof(buffer); i++) {
    buffer[i] = 'A';
  }
}

void loop() {
  Serial.println("Attempting to connect to TCP server...");

  if (client.connect(serverAddress, serverPort)) {
    Serial.println("Connected! Starting 10-second throughput test...");

    unsigned long startTime = millis();
    unsigned long bytesSent = 0;

    // Pump data for exactly 10 seconds
    while (millis() - startTime < 10000) {
      int bytes = client.write(buffer, sizeof(buffer));
      if (bytes > 0) {
        bytesSent += bytes;
      }
    }

    client.stop();

    // Calculate Mbps: (Bytes * 8 bits) / 1,000,000 / 10 seconds
    float mbps = (bytesSent * 8.0) / 1000000.0 / 10.0;

    Serial.print("Test complete. Speed: ");
    Serial.print(mbps);
    Serial.println(" Mbps\n");
  } else {
    Serial.println(
        "Connection failed. Is the server running? Retrying in 5s...");
    delay(5000);
  }

  delay(5000); // Wait before running the test again
}
