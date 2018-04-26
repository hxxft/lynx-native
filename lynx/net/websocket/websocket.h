#ifndef LYNX_NET_WEBSOCKET_WEB_SOCKET_H_
#define LYNX_NET_WEBSOCKET_WEB_SOCKET_H_

#include "net/socket/socket.h"
#include "net/websocket/websocket.h"
#include "net/websocket/websocket_frame.h"
#include "net/websocket/websocket_hand_shake.h"
#include "net/websocket/websocket_stream.h"
#include "base/io_buffer.h"

#include <queue>
#include <stdint.h>

namespace net {
    class WebSocket : public WebSocketHandShake::Delegate {
    public:
        enum WebSocketState {
            WS_STATE_NONE,
            WS_STATE_CONNECTING,
            WS_STATE_CONNECTED,
            WS_STATE_SEND_CLOSED,
            WS_STATE_RECV_CLOSED,
            WS_STATE_CLOSE_WAIT,
            WS_STATE_CLOSED,
        };
        
        enum WebSocketListenerState {
            WSL_STATE_OPENED,
            WSL_STATE_REC_MSG,
        };
        
        class Listener {
        public:
            virtual void OnOpen(const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size) = 0;
            virtual void OnMessage(const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size) = 0;
            virtual void OnClosing() = 0;
            virtual void OnClosed() = 0;
            virtual void OnFailure() = 0;
        };
        
        WebSocket(Listener* listener, const std::string& address, int port);
        
        int Connect();
        
        void SendText(const std::string& text);
        
        void SendBytes();
        
        void Close();
        
    protected:
        friend class WebSocketHandShake;
        
        virtual void OnHandShakeSuccessed(Socket* socket, const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size);
        virtual void OnHandShakeFailed();

        
    private:

        void SendFrame(bool fin, WebSocketFrameHeader::OpCode op_code, const base::ScopedRefPtr<base::IOBuffer>& buffer, size_t size);
        void SendFrames();
        
        void OnWriteDone(int result);
        
        void ReadFrames();
        void OnReadDone(int result);
        WebSocketState HandleFrame(base::ScopedPtr<WebSocketFrame> frame);
        WebSocketState HandleFrameByState(const WebSocketFrameHeader::OpCode opcode,
                                          bool fin, const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                                        uint64_t size);
        
        WebSocketState HandleDataFrame(WebSocketFrameHeader::OpCode opcode,
                                     bool fi,
                                       const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                                     uint64_t size);
        
        WebSocketState Fail(const std::string& message,
                             uint16_t code,
                             const std::string& reason) ;
        void SendClose(uint16_t code,
                       const std::string& reason);
        
        bool ParseClose(const base::ScopedRefPtr<base::IOBuffer>& buffer,
                        uint64_t size,
                        uint16_t* code,
                        std::string* reason,
                        std::string* message);
        
        WebSocketHandShake hand_shake_;
        
        base::ScopedPtr<WebSocketStream> stream_;
        
        base::ScopedPtr<Listener> listener_;
        
        std::queue<base::ScopedPtr<WebSocketFrame>> send_queue_;
        std::queue<base::ScopedPtr<WebSocketFrame>> next_send_queue_;
        
        std::vector<base::ScopedPtr<WebSocketFrame>> read_frames_;
        
        WebSocketState state_;
    private:
        DISALLOW_COPY_AND_ASSIGN(WebSocket);
    };
}

#endif  // LYNX_NET_WEBSOCKET_WEB_SOCKET_H_
