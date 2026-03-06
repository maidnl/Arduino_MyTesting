

#include "atest_log.h"
#include "atest_wire.h"
#include <cstdint>

template <class T> int test_check_buffer(T *obtained, T *expected, size_t dim) {
  for (size_t i = 0; i < dim; i++) {
    if (*(expected + i) != *(obtained + i)) {
      return 1;
    }
  }
  return 0;
}
