
#include "net/socket/socket_posix.h"
#include "base/poller/utility.h"
#include "base/threading/message_loop.h"

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>

#include "net/base/net_errors.h"

namespace net {
    
    namespace {
        
//        int MapAcceptError(int os_error) {
//            switch (os_error) {
//                case ECONNABORTED:
//                    return ERR_IO_PENDING;
//                default:
//                    return MapSystemError(os_error);
//            }
//        }
        
        int MapConnectError(int os_error) {
            switch (os_error) {
                case EINPROGRESS:
                    return ERR_IO_PENDING;
                case EACCES:
                    return ERR_NETWORK_ACCESS_DENIED;
                case ETIMEDOUT:
                    return ERR_CONNECTION_TIMED_OUT;
                default: {
                    int net_error = MapSystemError(os_error);
                    if (net_error == ERR_FAILED)
                        return ERR_CONNECTION_FAILED;
                    return net_error;
                }
            }
        }
        
    }  // namespace

    SocketPosix::SocketPosix(const std::string& address, int port)
        :address_(address),
        port_(port),
        socket_fd_(-1),
        waiting_connect_(false),
        read_buffer_(nullptr),
        read_buffer_len_(0),
        read_callback_(),
        write_buffer_(nullptr),
        write_buffer_len_(0),
        write_callback_(),
        accept_callback_(){
        
    }
    
    SocketPosix::~SocketPosix() {
        Close();
    }
    
    int SocketPosix::Open(int address_family) {
        socket_fd_ = socket(address_family, SOCK_STREAM, address_family == AF_UNIX ? 0 : IPPROTO_TCP);
        if(socket_fd_ < 0) {
            return MapSystemError(errno);
        }
        
        
        if(!base::SetNonBlocking(socket_fd_)) {
            int rv = MapSystemError(errno);
            Close();
            return rv;
        }
        return OK;
    }
    
    int SocketPosix::Bind() {
        struct sockaddr_in addr;
        
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_);
        if(inet_pton(AF_INET, address_.c_str(), &addr.sin_addr) <= 0){
            return errno;
        }
        int rv = bind(socket_fd_, (struct sockaddr*)&addr, sizeof(addr));
        
        if (rv < 0) {
            return MapSystemError(errno);
        }
        
        return OK;
    }

    int SocketPosix::Connect(base::CompletionCallbackBase* callback) {
        int rv = DoConnect();
        if(rv != ERR_IO_PENDING)
            return rv;
        base::MessageLoopForIO::current()->WatchFileDescriptor(
                                          new base::FileDescriptor(this, socket_fd_, FD_EVENT_OUT));
        
        write_callback_.Reset(callback);
        waiting_connect_ = true;
        return ERR_IO_PENDING;
    }
    
    int SocketPosix::Read(base::IOBuffer* buffer, int buf_len, base::CompletionCallbackBase* callback) {
        int rv = DoRead(buffer, buf_len);
        if(rv != ERR_IO_PENDING)
            return rv;
        base::MessageLoopForIO::current()->WatchFileDescriptor(
                                          new base::FileDescriptor(this, socket_fd_, FD_EVENT_IN));
        
        read_buffer_ = buffer;
        read_buffer_len_ = buf_len;
        read_callback_.Reset(callback);
        return ERR_IO_PENDING;
    }
    
    int SocketPosix::Write(base::IOBuffer* buffer, int buf_len, base::CompletionCallbackBase* callback) {
        
        int rv = DoWrite(buffer, buf_len);
        if(rv != ERR_IO_PENDING)
            return rv;
        base::MessageLoopForIO::current()->WatchFileDescriptor(
                                        new base::FileDescriptor(this, socket_fd_, FD_EVENT_OUT));
        write_buffer_ = buffer;
        write_buffer_len_ = buf_len;
        write_callback_.Reset(callback);
        return MapSystemError(errno);
    }
    
    int SocketPosix::Close() {
        if (socket_fd_ != kInvalidSocket) {
            if (close(socket_fd_) < 0){}
                //PLOG(ERROR) << "close() returned an error, errno=" << errno;
            socket_fd_ = kInvalidSocket;
        }
        return MapSystemError(errno);
    }
    
    int SocketPosix::DoConnect() {
        struct sockaddr_in servaddr;
        
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port_);
        if(inet_pton(AF_INET, address_.c_str(), &servaddr.sin_addr) <= 0){
            return MapConnectError(errno);
        }
        int rv = connect(socket_fd_, (struct sockaddr*)&servaddr, sizeof(servaddr));
        
       return rv == 0 ? OK : MapConnectError(errno);
    }
    
    int SocketPosix::DoRead(base::IOBuffer* buffer, int buf_len) {
        int rv = (int)read(socket_fd_, buffer->data(), buf_len);
        return rv >= 0 ? rv : MapSystemError(errno);
    }
    
    int SocketPosix::DoWrite(base::IOBuffer* buffer, int buf_len) {
        int rv = (int)write(socket_fd_, buffer->data(), buf_len);
        return rv >= 0 ? rv : MapSystemError(errno);
    }

    void SocketPosix::ConnectCompleted() {
        int os_error = 0;
        
        socklen_t len = sizeof(os_error);
        if (getsockopt(socket_fd_, SOL_SOCKET, SO_ERROR, &os_error, &len) == 0) {
            // TCPSocketPosix expects errno to be set.
            errno = os_error;
        }
        
        int rv = MapConnectError(errno);
        if (rv == ERR_IO_PENDING)
            return;
        
        base::MessageLoopForIO::current()->RemoveFileDescriptor(socket_fd_);
        waiting_connect_ = false;
        write_callback_->Run(rv);
    }
    
    void SocketPosix::ReadCompleted() {
        int rv = DoRead(read_buffer_.Get(), read_buffer_len_);
        if (rv == ERR_IO_PENDING)
            return;
        base::MessageLoopForIO::current()->RemoveFileDescriptor(socket_fd_);
        read_buffer_ = nullptr;
        read_buffer_len_ = 0;
        read_callback_->Run(rv);
    }
    
    void SocketPosix::WriteCompleted() {
        int rv = DoWrite(write_buffer_.Get(), write_buffer_len_);
        if (rv == ERR_IO_PENDING)
            return;
        base::MessageLoopForIO::current()->RemoveFileDescriptor(socket_fd_);
        write_buffer_ = nullptr;
        write_buffer_len_ = 0;
        write_callback_->Run(rv);
    }
    
    void SocketPosix::OnFileCanRead(int fd) {
        if(accept_callback_.Get() != nullptr) {

        }else {
            ReadCompleted();
        }
    }
    
    void SocketPosix::OnFileCanWrite(int fd) {
        if (waiting_connect_) {
            ConnectCompleted();
        } else {
            WriteCompleted();
        }
    }
}
