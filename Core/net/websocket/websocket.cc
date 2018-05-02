#include "net/websocket/websocket.h"
#include "base/base64.h"
#include "base/io_buffer.h"
#include "base/task/callback.h"
#include "base/big_endian.h"
#include "net/base/net_errors.h"
#include <sys/socket.h>

namespace net {
    namespace {
        const int kBufferSize = 1024;
        const size_t kWebSocketCloseCodeLength = 2;
        void GetFrameTypeForOpcode(WebSocketFrameHeader::OpCode opcode,
                                   std::string* name) {
            switch (opcode) {
                case WebSocketFrameHeader::kOpCodeText:    // fall-thru
                case WebSocketFrameHeader::kOpCodeBinary:  // fall-thru
                case WebSocketFrameHeader::kOpCodeContinuation:
                    *name = "Data frame";
                    break;
                    
                case WebSocketFrameHeader::kOpCodePing:
                    *name = "Ping";
                    break;
                    
                case WebSocketFrameHeader::kOpCodePong:
                    *name = "Pong";
                    break;
                    
                case WebSocketFrameHeader::kOpCodeClose:
                    *name = "Close";
                    break;
                    
                default:
                    *name = "Unknown frame type";
                    break;
            }
            
            return;
        }
    }
    
    
    WebSocket::WebSocket(Listener* listener, const std::string& address, int port)
        : hand_shake_(address, port, this), stream_(), listener_(listener), send_queue_(), next_send_queue_() {
        
    }
    
    void WebSocket::SendText(const std::string& text) {
        base::ScopedRefPtr<base::IOBuffer> buffer(new base::StringIOBuffer(text));
        SendFrame(true, WebSocketFrameHeader::kOpCodeText, buffer, text.length());
    }
    
    void WebSocket::SendBytes() {
        
    }
   
    int WebSocket::Connect() {
        return hand_shake_.ShakeHand();
    }
    
    void WebSocket::OnHandShakeSuccessed(Socket* socket, const base::ScopedRefPtr<base::IOBuffer>& data_buffer, uint64_t size) {
        state_ = WS_STATE_CONNECTED;
        
        stream_.Reset(new WebSocketStream(socket));
        if(listener_.Get()) {
            listener_->OnOpen(data_buffer, size);
        }
        
        SendFrames();
        ReadFrames();
    }
    
    void WebSocket::OnHandShakeFailed() {
        if(listener_.Get()) {
            listener_->OnFailure();
        }
        
    }
    
    void WebSocket::SendFrame(bool fin, WebSocketFrameHeader::OpCode op_code, const base::ScopedRefPtr<base::IOBuffer>& buffer, size_t size) {
        base::ScopedPtr<WebSocketFrame> frame(new WebSocketFrame(op_code));
        WebSocketFrameHeader& header = frame->header_;
        header.final_ = fin;
        header.masked_ = true;
        header.payload_length_ = size;
        frame->data_ = buffer;
        
        if (!send_queue_.empty()) {
            next_send_queue_.push(std::move(frame));
            return ;
        }
        
        send_queue_.push(std::move(frame));
        return SendFrames();
    }
    
    void WebSocket::SendFrames() {
        if(stream_.Get() == nullptr || send_queue_.empty()) return;
        int result = OK;
        do {
            // This use of base::Unretained is safe because this object owns the
            // WebSocketStream and destroying it cancels all callbacks.
            result = stream_->WriteFrames(send_queue_.front(), new base::CompletionCallback<WebSocket>(&WebSocket::OnWriteDone, this));
            send_queue_.pop();
            if (result != ERR_IO_PENDING) {
                return OnWriteDone(result);
            }
        } while (result == OK);
        return ;
    }
    
    void WebSocket::OnWriteDone(int result) {
        switch (result) {
            case OK:
                if(!send_queue_.empty()) {
                    return SendFrames();
                } else if (!next_send_queue_.empty()) {
                    send_queue_ = std::move(next_send_queue_);
                    return SendFrames();
                }
                return ;
                
            default:
                stream_->Close();
                stream_.Reset();
                return ;
        }
    }
    
    void WebSocket::ReadFrames() {
        int result = OK;
        while (result == OK) {
            // This use of base::Unretained is safe because this object owns the
            // WebSocketStream, and any pending reads will be cancelled when it is
            // destroyed.
            result = stream_->ReadFrames(&read_frames_,
                                         new base::CompletionCallback<WebSocket>(&WebSocket::OnReadDone, this));
            if (result != ERR_IO_PENDING) {
                OnReadDone(result);
            }
        }
        return;
    }
    
    void WebSocket::OnReadDone(int result) {
        switch (result) {
            case OK:
                for (size_t i = 0; i < read_frames_.size(); ++i) {
                    if (HandleFrame(std::move(read_frames_[i])) != WS_STATE_CONNECTED)
                        return;
                }
                read_frames_.clear();
                ReadFrames();
                break;
                
            case ERR_WS_PROTOCOL_ERROR:
                Fail("Invalid frame header",
                                   kWebSocketErrorProtocolError,
                                   "WebSocket Protocol Error");
                break;
            default:
                
                stream_->Close();
                state_ = WS_STATE_CLOSED;

                if(listener_.Get()) {
                    listener_->OnClosing();
                }
                break;
        }
    }
    
    WebSocket::WebSocketState WebSocket::HandleFrame(base::ScopedPtr<WebSocketFrame> frame) {
        if (frame->header_.masked_) {
            // RFC6455 Section 5.1 "A client MUST close a connection if it detects a
            // masked frame."
            return Fail( "A server must not mask any frames that it sends to the ",
                         kWebSocketErrorProtocolError,
                         "Masked frame from server");
        }
        const WebSocketFrameHeader::OpCode opcode = frame->header_.opcode_;

        if (frame->header_.reserved1_ || frame->header_.reserved2_ ||
            frame->header_.reserved3_) {
             return Fail( "One or more reserved bits are on",
                        kWebSocketErrorProtocolError,
                        "Invalid reserved bit");
        }
        
        return HandleFrameByState(opcode, frame->header_.final_, frame->data_, frame->header_.payload_length_);
    }
    
    WebSocket::WebSocketState WebSocket::HandleFrameByState(const WebSocketFrameHeader::OpCode opcode,
                                                 bool fin,
                                                 const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                                                 uint64_t size) {
        if (state_ == WS_STATE_CLOSE_WAIT) {
            std::string frame_name;
            GetFrameTypeForOpcode(opcode, &frame_name);
            return Fail(frame_name + " received after close", kWebSocketErrorProtocolError, "");
        }
        switch (opcode) {
            case WebSocketFrameHeader::kOpCodeText:  // fall-thru
            case WebSocketFrameHeader::kOpCodeBinary:
            case WebSocketFrameHeader::kOpCodeContinuation:
                return HandleDataFrame(opcode, fin, data_buffer, size);
                
            case WebSocketFrameHeader::kOpCodePing:
                if (state_ == WS_STATE_CONNECTED)
                    SendFrame(true, WebSocketFrameHeader::kOpCodePong, data_buffer, size);
                return state_;
                
            case WebSocketFrameHeader::kOpCodePong:
                return WS_STATE_CONNECTED;
                
            case WebSocketFrameHeader::kOpCodeClose: {
                uint16_t code = kWebSocketNormalClosure;
                std::string reason;
                std::string message;
                if (!ParseClose(data_buffer, size, &code, &reason, &message)) {
                    return Fail(message, code, reason);
                }
                switch (state_) {
                    case WS_STATE_CONNECTED:
                        state_ = WS_STATE_RECV_CLOSED;
                        SendClose(code, reason);
                        break;
                        
                    case WS_STATE_SEND_CLOSED:
                        state_ = WS_STATE_CLOSE_WAIT;
                        break;
                    default:
                        break;
                }
                return WS_STATE_CONNECTED;
            }
                
            default:
                return Fail("Unrecognized frame opcode",
                            kWebSocketErrorProtocolError,
                            "Unknown opcode");
        }
    }
    
    WebSocket::WebSocketState WebSocket::HandleDataFrame(
                                                         WebSocketFrameHeader::OpCode opcode,
                                                         bool fin,
                                                         const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                                                         uint64_t size) {
        if(state_ == WS_STATE_CONNECTED && listener_.Get()) {
            listener_->OnMessage(data_buffer, size);
        }
        return state_;
    }
    
    WebSocket::WebSocketState WebSocket::Fail(const std::string& message,
                                    uint16_t code,
                                    const std::string& reason) {
        if(state_ == WS_STATE_CONNECTED) {
            SendClose(code, reason);
        }

        if(listener_.Get()) {
            listener_->OnFailure();
        }
        return state_;
    }
    
    void WebSocket::SendClose(uint16_t code,
                                             const std::string& reason) {
        base::ScopedRefPtr<base::IOBuffer> body;
        uint64_t size = 0;
        state_ = WS_STATE_CLOSE_WAIT;
        if (code == kWebSocketErrorNoStatusReceived) {
            body = new base::IOBuffer(size);
        } else {
            const size_t payload_length = kWebSocketCloseCodeLength + reason.length();
            body = new base::IOBuffer(payload_length);
            size = payload_length;
            base::WriteBigEndian(body->data(), code);
            std::copy(reason.begin(), reason.end(), body->data() + kWebSocketCloseCodeLength);
        }
        SendFrame(true, WebSocketFrameHeader::kOpCodeClose, body, size);
    }
    
    bool WebSocket::ParseClose(const base::ScopedRefPtr<base::IOBuffer>& buffer,
                                      uint64_t size,
                                      uint16_t* code,
                                      std::string* reason,
                                      std::string* message) {
        reason->clear();
        if (size < kWebSocketCloseCodeLength) {
            if (size == 0U) {
                *code = kWebSocketErrorNoStatusReceived;
                return true;
            }
            
            *code = kWebSocketErrorProtocolError;
            *message =
            "Received a broken close frame containing an invalid size body.";
            return false;
        }
        
        const char* data = buffer->data();
        uint16_t unchecked_code = 0;
        base::ReadBigEndian(data, &unchecked_code);
        static_assert(sizeof(unchecked_code) == kWebSocketCloseCodeLength,
                      "they should both be two bytes");
        
        switch (unchecked_code) {
            case kWebSocketErrorNoStatusReceived:
            case kWebSocketErrorAbnormalClosure:
            case kWebSocketErrorTlsHandshake:
                *code = kWebSocketErrorProtocolError;
                *message =
                "Received a broken close frame containing a reserved status code.";
                return false;
                
            default:
                *code = unchecked_code;
                break;
        }
        
        std::string text(data + kWebSocketCloseCodeLength, data + size);
        reason->swap(text);
        return true;
    }
    
}
