#ifndef LYNX_NET_SOCKET_SOCKET_H_
#define LYNX_NET_SOCKET_SOCKET_H_

#include "base/task/callback.h"
#include "base/io_buffer.h"

#include <string>

namespace net {
    class Socket {
    public:
        Socket(){}
        virtual ~Socket(){}
        virtual int Open(int address_family) = 0;
        virtual int Bind() = 0;
        virtual int Connect(base::CompletionCallbackBase* callback) = 0;
        virtual int Read(base::IOBuffer* buffer, int buf_len, base::CompletionCallbackBase* callback) = 0;
        virtual int Write(base::IOBuffer* buffer, int buf_len, base::CompletionCallbackBase* callback) = 0;
        virtual int Close() = 0;
        
        static Socket* CreateSocket(const std::string& address, int port);
    };
}

#endif
