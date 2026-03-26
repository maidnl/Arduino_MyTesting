
#ifndef ASKETCH_CONFIG
#define ASKETCH_CONFIG

#if defined(ARDUINO_NANO33BLE)

/* ****************************************************************************
 *                             NANO 33 BLE PARAMETERS
 * ************************************************************************* */

/* ___________________________________________________________WIRE PARAMETERS */

#define AT_WIRE Wire

#define AT_WIRE_SLAVE_ADDRESS 0x0A

#define AT_WIRE_TIMEOUT_ms 50
#define AT_WIRE_MASTER_TASK_ms 1000
#define AT_WIRE_MASTER_DELAY_ms 100

#define AT_WIRE_SDA_PIN A4
#define AT_WIRE_SCL_PIN A5

const uint8_t msg[] = {'M', 'E', 'S', 'S', 'A', 'G', 'G', 'I', 'O'};

const uint8_t req[] = {'R', 'I', 'C', 'H', 'I', 'E', 'S', 'T', 'A'};
const uint8_t expected[] = {'R', 'i', 's', 'p', 'o', 's', 't', 'a'};

/* ________________________________________________ DIGITAL OUTPUT PARAMETERS */

#define AT_DIGITAL_OUT_TASK_ms 50

/* __________________________________________________  ANALOG INPUT PARAMETERS*/

#define AT_ANALOG_IN_TASK_ms 1000

#elif defined(ARDUINO_GIGA)

/* ___________________________________________________________WIRE PARAMETERS */

#define AT_WIRE Wire2

#define AT_WIRE_SLAVE_ADDRESS 0x0A

#define AT_WIRE_TIMEOUT_ms 50
#define AT_WIRE_MASTER_TASK_ms 1000
#define AT_WIRE_MASTER_DELAY_ms 100

#define AT_WIRE_SDA_PIN 9
#define AT_WIRE_SCL_PIN 8

const uint8_t msg[] = {'M', 'E', 'S', 'S', 'A', 'G', 'G', 'I', 'O'};

const uint8_t req[] = {'R', 'I', 'C', 'H', 'I', 'E', 'S', 'T', 'A'};
const uint8_t expected[] = {'R', 'i', 's', 'p', 'o', 's', 't', 'a'};

/* ________________________________________________ DIGITAL OUTPUT PARAMETERS */

#define AT_DIGITAL_OUT_TASK_ms 50

/* __________________________________________________  ANALOG INPUT PARAMETERS*/

#define AT_ANALOG_IN_TASK_ms 1000
#endif

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* _______________________________________________________COMMON FUNCTIONS    */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

static bool i2c_initialized = false;
void i2c_set_initialized(bool v) { i2c_initialized = v; }

void i2c_initialize() {
  if (!i2c_initialized) {
    i2c_init<Wire_t>(AT_WIRE, 0);
    i2c_initialized = true;
  }
}

void i2c_deinitialize() {
  if (i2c_initialized) {
    AT_WIRE.end();
    i2c_initialized = false;
  }
}
#endif
