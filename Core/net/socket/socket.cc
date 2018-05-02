#include "net/socket/socket.h"
#include "net/socket/socket_posix.h"

namespace net {
    Socket* Socket::CreateSocket(const std::string& address, int port) {
        return new SocketPosix(address, port);
    }
}
