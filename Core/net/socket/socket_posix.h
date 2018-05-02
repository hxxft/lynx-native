#ifndef LYNX_NET_SOCKET_SOCKET_POSIX_H_
#define LYNX_NET_SOCKET_SOCKET_POSIX_H_

#include "base/poller/poller.h"
#include "base/io_buffer.h"
#include "base/task/callback.h"
#include "net/socket/socket.h"

namespace net {
    const int kInvalidSocket = -1;
    
    class SocketPosix : public Socket, public base::Poller::Watcher {
    public:
        explicit SocketPosix(const std::string& address, int port);
        virtual ~SocketPosix();
        
        int Open(int address_family);
        int Bind();
        int Connect(base::CompletionCallbackBase* callback);
        int Read(base::IOBuffer* buffer, int buf_len, base::CompletionCallbackBase* callback);
        int Write(base::IOBuffer* buffer, int buf_len, base::CompletionCallbackBase* callback);
        int Close();
        
        void OnFileCanRead(int fd);
        void OnFileCanWrite(int fd);
    private:
        int DoConnect();
        int DoRead(base::IOBuffer* buffer, int buf_len);
        int DoWrite(base::IOBuffer* buffer, int buf_len);
        
        void ConnectCompleted();
        void ReadCompleted();
        void WriteCompleted();
        
        std::string address_;
        int port_;
        
        int socket_fd_;
        
        bool waiting_connect_;
        
        base::ScopedRefPtr<base::IOBuffer> read_buffer_;
        int read_buffer_len_;
        base::ScopedPtr<base::CompletionCallbackBase> read_callback_;
        
        base::ScopedRefPtr<base::IOBuffer> write_buffer_;
        int write_buffer_len_;
        base::ScopedPtr<base::CompletionCallbackBase> write_callback_;
        
        base::ScopedPtr<base::CompletionCallbackBase> accept_callback_;
    private:
        DISALLOW_COPY_AND_ASSIGN(SocketPosix);
    };
}

#endif  // LYNX_NET_SOCKET_SOCKET_POSIX_H_
