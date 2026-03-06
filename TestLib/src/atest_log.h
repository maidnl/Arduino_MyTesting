#ifndef A_TEST_LOG_H
#define A_TEST_LOG_H

#include "Arduino.h"

typedef enum { LOG, WRN, ERR, OK, FAIL } LogType_t;
int alog(const char *fmt, ...);
int alogln(const char *fmt, ...);

#endif // !A_TEST_LOG_H
