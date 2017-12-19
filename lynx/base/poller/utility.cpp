#include "base/poller/utility.h"

#include <fcntl.h>

namespace base {
    bool SetNonBlocking(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1)
            return false;
        if (flags & O_NONBLOCK)
            return true;
        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
            return false;
        return true;
    }
}
