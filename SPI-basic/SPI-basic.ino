#include <SPI.h>

// --- Configuration ---
const int csPin = 10; // Digital pin 10 used for Chip Select
const char message[16] = "Hello SPI Slave"; 
const int cycleDelay = 2000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // 1. Initialize the Serial monitor for debugging
  Serial.begin(115200);

  // Wait for the serial port to connect (useful for native USB boards like the
  // GIGA)
  while (!Serial) {
    ;
  }
  Serial.println("SPI Master Initialization Starting...");

  // 2. Configure the Chip Select (CS) pin
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);

  // 3. Initialize the SPI1 bus
  SPI1.begin();

  Serial.println("SPI1 Initialized. Ready to transmit.");
  Serial.println("------------------------------------");
}

void blink() {
  static bool st = false;
  if (st) {
    digitalWrite(LED_BUILTIN, HIGH);
    st = false;
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    st = true;
  }
}

void loop() {
  Serial.println("Starting new SPI transaction...");

  uint8_t transferBuffer[16];
  memset(transferBuffer,0x00,16);
  memcpy(transferBuffer,message,16);

  digitalWrite(csPin, LOW);
  delay(1);

  SPI1.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  SPI1.transfer(transferBuffer, 16);
  SPI1.endTransaction();
  delay(1);
  digitalWrite(csPin, HIGH);
 
  Serial.print("Received from Slave: ");
  for (int i = 0; i < 16; i++) {
    Serial.print((char)transferBuffer[i]);
  }
  Serial.println("\nTransaction finished.");
  Serial.println("------------------------------------");
 
  

 

  // 6. Wait before sending the next cyclic message
  delay(cycleDelay);

  blink();
}
