#include "net/websocket/websocket_hand_shake.h"
#include "base/base64.h"
#include "base/rand_util.h"

#include "net/base/net_errors.h"
#include <sys/socket.h>

namespace net {
    namespace {
        const int kHandShakeBufferSize = 1024;
        
        int GetHandleShakeRequest(base::IOBuffer* buffer) {
            char sec_ukey[20];
            base::RandomBuffer(sec_ukey, 20);
            size_t key_length = 0;
            base64_encode(NULL, &key_length, NULL, 20);
            char *sec_key = (char *)malloc(key_length);
            if (!sec_key) {
                return -1;
                //return self->on_error(self, "Out of memory");
            }
            if (base64_encode((unsigned char *)sec_key, &key_length,
                              (const unsigned char *)sec_ukey, 20)) {
                free(sec_key);
                return -1;
                //return self->on_error(self, "base64_encode failed");
            }
            
            
            
            char* out_buffer = buffer->data();
            
            out_buffer += sprintf(out_buffer,
                                  "GET /mdevices/page/android HTTP/1.1\r\n"
                                  "Upgrade: WebSocket\r\n"
                                  "Connection: Upgrade\r\n"
                                  "Sec-WebSocket-Version: 13\r\n"
                                  "Sec-WebSocket-Key: %s\r\n", sec_key);
            
            free(sec_key);
            //        if (protocol) {
            //            buffer += sprintf(buffer, "Sec-WebSocket-Protocol: %s\r\n",
            //                                protocol);
            //        }
            //        if (host) {
            //            buffer += sprintf(buffer, "Host: %s\r\n", host);
            //        }
            //        if (origin) {
            //            buffer += sprintf(buffer, "Origin: %s\r\n", origin);
            //        }
            out_buffer += sprintf(out_buffer, "\r\n");
            
            return out_buffer - buffer->data();
            
        }
    }
    
    WebSocketHandShake::WebSocketHandShake(const std::string& address, int port, Delegate* delegate)
        :socket_(Socket::CreateSocket(address, port)),
            next_hand_shake_state_(HAND_SHAKE_STATE_NONE),
            delegate_(delegate) {
        
    }
    
    WebSocketHandShake::~WebSocketHandShake() {
        
    }
    
    int WebSocketHandShake::ShakeHand() {
        next_hand_shake_state_ = HAND_SHAKE_STATE_CONNECT;
        int rv = DoHandShakeLoop(0);
        return rv;
    }
    
    int WebSocketHandShake::DoHandShakeLoop(int result) {
        int rv = 0;
        do {
            HandleShakeState state = next_hand_shake_state_;
            next_hand_shake_state_ = HAND_SHAKE_STATE_NONE;
            
            switch (state) {
                case HAND_SHAKE_STATE_CONNECT:
                    rv = DoHandShakeConnect();
                    break;
                case HAND_SHAKE_STATE_SEND_HAND_SHAKE_REQUEST:
                    rv = SendHandShakeRequest();
                    break;
                case HAND_SHAKE_STATE_READ_HAND_SHAKE_RESPONSE:
                    rv = ReadHandShakeResponse();
                    break;
                case HAND_SHAKE_STATE_HAND_SHAKE_COMPLETE:
                    rv = HandShakeCompleted();
                    return rv;
                default:
                    break;
            }
        }while(rv != ERR_IO_PENDING);
        return rv;
    }
    
    int WebSocketHandShake::DoHandShakeConnect() {
        next_hand_shake_state_ = HAND_SHAKE_STATE_SEND_HAND_SHAKE_REQUEST;
        int rv = socket_->Open(AF_INET);
        if(rv != 0)
            return rv;
        
        return socket_->Connect(new base::CompletionCallback<WebSocketHandShake>(&WebSocketHandShake::DidHandShakeLoop, this));
    }
    
    int WebSocketHandShake::SendHandShakeRequest() {
        next_hand_shake_state_ = HAND_SHAKE_STATE_READ_HAND_SHAKE_RESPONSE;
        
        base::IOBuffer* buffer = new base::IOBuffer(kHandShakeBufferSize);
        int out_length = GetHandleShakeRequest(buffer);
        int rv = socket_->Write(buffer, out_length, new base::CompletionCallback<WebSocketHandShake>(&WebSocketHandShake::DidHandShakeLoop, this));
        if(rv != ERR_IO_PENDING)
            buffer->Release();
        return rv;
    }
    
    
    int WebSocketHandShake::ReadHandShakeResponse() {
        next_hand_shake_state_ = HAND_SHAKE_STATE_HAND_SHAKE_COMPLETE;
        read_buffer_ = new base::IOBuffer(kHandShakeBufferSize);
        int rv = socket_->Read(read_buffer_.Get(), kHandShakeBufferSize, new base::CompletionCallback<WebSocketHandShake>(&WebSocketHandShake::DidHandShakeLoop, this));
        return rv;
    }
    
    int WebSocketHandShake::HandShakeCompleted() {
        next_hand_shake_state_ = HAND_SHAKE_STATE_NONE;
        delegate_->OnHandShakeSuccessed(socket_.Release(), read_buffer_, 0);
        return OK;
    }
    
    void WebSocketHandShake::DidHandShakeLoop(int result) {
        DoHandShakeLoop(result);
    }    
   
}
