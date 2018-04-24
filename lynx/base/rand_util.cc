#include "base/rand_util.h"

#include <stdlib.h>
#include <time.h>

namespace base {
void RandomBuffer(char* buffer, size_t length) {
  static bool seeded = false;
  if (!seeded) {
    seeded = true;
    // could fread from /dev/random
    srand(time(NULL));
  }
  size_t i;
  for (i = 0; i < length; i++) {
    buffer[i] = (char)rand();
  }
}
}  // namespace base
