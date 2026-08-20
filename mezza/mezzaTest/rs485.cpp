#include "rs485.h"
// ==========================================
#include <sys/stat.h>

// Provide dummy syscall stubs to satisfy the newlib linker
extern "C" {
int _close(int file) { return -1; }

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }

int _read(int file, char *ptr, int len) { return 0; }

// Sometimes _write is also required when TLS errno issues appear
int _write(int file, char *ptr, int len) { return len; }

// --------------------------------------------------------
// Memory allocation stub (_sbrk)
// --------------------------------------------------------
// Define a 4KB heap buffer. Increase this if your
// application uses heavily dynamic memory or large Strings.
#define HEAP_SIZE 4096
static char heap[HEAP_SIZE];
static char *heap_end = heap;

void *_sbrk(int incr) {
  char *prev_heap_end = heap_end;

  // Check if we have enough space left in our simulated heap
  if (heap_end + incr > heap + HEAP_SIZE) {
    // Out of memory
    return (void *)-1;
  }

  heap_end += incr;
  return (void *)prev_heap_end;
}
}
uint16_t counter = 0;

void mezza_test_init_rs485() {
  Serial.begin(115200);

  while (!Serial)
    ;

  Serial.println("Arduino Core Zephyr - Modbus RTU Example");

  if (DEVICE_ADDRESS == 0) {
    // ---------------------------------
    // SETUP: MASTER (CLIENT)
    // ---------------------------------
    Serial.println("Role: MASTER");
    if (!ModbusRTUClient.begin(MODBUS_BAUD)) {
      Serial.println("Failed to start Modbus RTU Client!");
      while (1)
        ;
    }
    RS485.setDelays(50, 2000);
  } else {
    // ---------------------------------
    // SETUP: SLAVE (SERVER)
    // ---------------------------------
    Serial.print("Role: SLAVE - Address: ");
    Serial.println(DEVICE_ADDRESS);

    // Start Modbus Server on the assigned address
    if (!ModbusRTUServer.begin(RS485, DEVICE_ADDRESS, MODBUS_BAUD)) {
      Serial.println("Failed to start Modbus RTU Server!");
      while (1)
        ;
    }
    RS485.setDelays(50, 2000);

    // Configure 2 Holding Registers at address 0x00
    // Register 0: Will hold our "Text" encoded as 16-bits
    // Register 1: Will hold our incrementing counter
    ModbusRTUServer.configureHoldingRegisters(0x00, 2);
  }
}

void mezza_test_rs485() {
  if (DEVICE_ADDRESS == 0) {
    static unsigned long t = 0;
    if (millis() - t > TASK_MODBUS_ms) {
      t = millis();

      // ---------------------------------
      // LOOP: MASTER (CLIENT)
      // ---------------------------------

      // Pack a 2-character text into a 16-bit register.
      // Example: 'O' = 0x4F, 'K' = 0x4B -> 0x4F4B
      uint16_t textData = ('O' << 8) | 'K';

      // Write data to Slaves 1, 2, and 3
      for (int slaveId = 1; slaveId <= 3; slaveId++) {

        ModbusRTUClient.beginTransmission(slaveId, HOLDING_REGISTERS, 0x00, 2);
        ModbusRTUClient.write(textData); // Write text to Register 0x00
        ModbusRTUClient.write(counter);  // Write counter to Register 0x01

        if (!ModbusRTUClient.endTransmission()) {
          Serial.print("Error sending to Slave ");
          Serial.print(slaveId);
          Serial.print(" - ");
          Serial.println(ModbusRTUClient.lastError());
        } else {
          Serial.print("Successfully sent to Slave ");
          Serial.print(slaveId);
          Serial.print(" | Text: 'OK' | Counter: ");
          Serial.println(counter);
        }
        // Small delay between targeting different slaves
        delay(DELAY_TX_ms);
      }

      counter++;
    }

  } else {
    // ---------------------------------
    // LOOP: SLAVE (SERVER)
    // ---------------------------------
    // Poll for Modbus requests from the Master
    ModbusRTUServer.poll();

    // Read the current values of our Holding Registers
    long textReg = ModbusRTUServer.holdingRegisterRead(0x00);
    long countReg = ModbusRTUServer.holdingRegisterRead(0x01);

    static long lastCount = -1;

    // If the counter has changed, we have received new data
    if (countReg != lastCount && textReg > 0) {
      // Decode the 16-bit register back into 2 characters
      char char1 = (textReg >> 8) & 0xFF;
      char char2 = textReg & 0xFF;

      Serial.print("Received Data -> Text: ");
      Serial.print(char1);
      Serial.print(char2);
      Serial.print(" | Counter: ");
      Serial.println(countReg);

      lastCount = countReg; // Update state so we don't spam the Serial monitor
    }
  }
}
