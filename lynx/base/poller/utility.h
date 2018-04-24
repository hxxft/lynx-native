#ifndef LYNX_BASE_POLLER_UTILITY_H_
#define LYNX_BASE_POLLER_UTILITY_H_

#include <fcntl.h>

namespace base {
bool SetNonBlocking(int fd);
}

#endif  // LYNX_BASE_POLLER_UTILITY_H_
