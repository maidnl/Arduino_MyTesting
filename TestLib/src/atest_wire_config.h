#ifndef ATEST_WIRE_CONFIG_H
#define ATEST_WIRE_CONFIG_H

typedef arduino::ZephyrI2C Wire_t;

#define WIRE Wire2

#define RX_BUFFER_DIM 10
#define TX_BUFFER_DIM 10
#define SLAVE_ADDRESS 0x0A
#define TIMEOUT 50
#define WRITE_TASK_PERIOD 1000
#define READ_TASK_PERIOD 1000

#define ARDUINO_WIRE_SDA_PIN 9
#define ARDUINO_WIRE_SCL_PIN 8

const uint8_t msg[] = {'M', 'E', 'S', 'S', 'A', 'G', 'G', 'I', 'O'};
const uint8_t req[] = {'R', 'I', 'C', 'H', 'I', 'E', 'S', 'T', 'A'};
const uint8_t expected[] = {'R', 'i', 's', 'p', 'o', 's', 't', 'a'};
#endif
