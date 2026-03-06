#include "atest_log.h"

#define MAX_LOG_DIM 256

static char log_buffer[MAX_LOG_DIM];

int alog(const char *fmt, ...) {

  // Clear the buffer
  memset(log_buffer, 0x00, MAX_LOG_DIM);

  va_list va;
  va_start(va, fmt);

  // Use vsnprintf instead of sprintf.
  // It takes the buffer, the max size, the format, and the va_list.
  int n = vsnprintf(log_buffer, MAX_LOG_DIM, fmt, va);

  va_end(va);

  // Print the correctly formatted string to the Serial monitor
  Serial.print(log_buffer);

  return n; // Returning the number of characters written is a good practic
}

int alogln(const char *fmt, ...) {

  memset(log_buffer, 0x00, MAX_LOG_DIM);
  va_list va;
  va_start(va, fmt);
  int n = vsnprintf(log_buffer, MAX_LOG_DIM, fmt, va);
  va_end(va);

  Serial.println(log_buffer);
  return n; // Returning the number of characters written is a good practic
}
