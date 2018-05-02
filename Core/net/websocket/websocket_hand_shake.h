#ifndef NET_WEBSOCKET_WEBSOCKET_HAND_SHAKE_H_
#define NET_WEBSOCKET_WEBSOCKET_HAND_SHAKE_H_

#include "net/socket/socket.h"
#include "base/scoped_ptr.h"

namespace net{
    class WebSocketHandShake {
    public:
        class Delegate {
        public:
            virtual void OnHandShakeSuccessed(Socket* socket, const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size) = 0;
            virtual void OnHandShakeFailed() = 0;
        };
        
        WebSocketHandShake(const std::string& address, int port, Delegate* delegate);
        ~WebSocketHandShake();
        
        int ShakeHand();
        
    private:
        enum HandleShakeState {
            HAND_SHAKE_STATE_CONNECT,
            HAND_SHAKE_STATE_SEND_HAND_SHAKE_REQUEST,
            HAND_SHAKE_STATE_READ_HAND_SHAKE_RESPONSE,
            HAND_SHAKE_STATE_HAND_SHAKE_COMPLETE,
            HAND_SHAKE_STATE_NONE,
        };
        
        
        int DoHandShakeLoop(int result);
        int DoHandShakeConnect();
        int SendHandShakeRequest();
        int ReadHandShakeResponse();
        int HandShakeCompleted();
        
        void DidHandShakeLoop(int result);
        
        base::ScopedPtr<Socket> socket_;
        
        HandleShakeState next_hand_shake_state_;
        
        Delegate* delegate_;
        
        base::ScopedRefPtr<base::IOBuffer> read_buffer_;
        
    };
}

#endif
