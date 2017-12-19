#ifndef LYNX_NET_WEBSOCKET_FRAME_H_
#define LYNX_NET_WEBSOCKET_FRAME_H_

#include <stdint.h>

#include "base/io_buffer.h"
#include "base/scoped_ptr.h"

namespace net {
    class WebSocketFrameHeader {
    public:
        typedef int OpCode;
        
        enum OpCodeType {
            kOpCodeContinuation = 0x0,
            kOpCodeText = 0x1,
            kOpCodeBinary = 0x2,
            kOpCodeDataUnused = 0x3,
            kOpCodeClose = 0x8,
            kOpCodePing = 0x9,
            kOpCodePong = 0xA,
            kOpCodeControlUnused = 0xB,
        };
        
        static bool IsKnownDataOpCode(OpCode opcode) {
            return opcode == kOpCodeContinuation || opcode == kOpCodeText || opcode == kOpCodeBinary;
        }
        
        static bool IsKnownControlOpCode(OpCode opcode) {
            return opcode == kOpCodeClose || opcode == kOpCodePing || opcode == kOpCodePong;
        }
        
        enum {
            kBaseHeaderSize = 2,
            kMaximumExtendedLengthSize = 8,
            kMaskingKeyLength = 4
        };
        
        explicit WebSocketFrameHeader(OpCode opCode)
            : final_(false),
            reserved1_(false),
            reserved2_(false),
            reserved3_(false),
            opcode_(opCode),
            masked_(false),
            payload_length_(0) {}
        
        base::ScopedPtr<WebSocketFrameHeader> Clone() const;
        
        void CopyFrom(const WebSocketFrameHeader& source);
        
        bool final_;
        bool reserved1_;
        bool reserved2_;
        bool reserved3_;
        OpCode opcode_;
        bool masked_;
        uint64_t payload_length_;
    private:
        DISALLOW_COPY_AND_ASSIGN(WebSocketFrameHeader);
    };
    
    struct WebSocketFrame {
        explicit WebSocketFrame(WebSocketFrameHeader::OpCode opcode) : header_(opcode) {}
        
        WebSocketFrameHeader header_;
        
        base::ScopedRefPtr<base::IOBuffer> data_;
    };
    
    struct WebSocketFrameChunk {
        WebSocketFrameChunk() : final_chunk(false) {}
        ~WebSocketFrameChunk() {}
        
        // Non-null |header| is provided only if this chunk is the first part of
        // a series of chunks.
        base::ScopedPtr<WebSocketFrameHeader> header;
        
        // Indicates this part is the last chunk of a frame.
        bool final_chunk;
        
        // |data| is always unmasked even if the frame is masked. |data| might be
        // null in the first chunk.
        base::ScopedRefPtr<base::IOBufferWithSize> data;
    };
    
    struct WebSocketMaskingKey {
        char key_[WebSocketFrameHeader::kMaskingKeyLength];
    };
    
    int GetWebSocketFrameHeaderSize(const WebSocketFrameHeader& header);
    
    int WriteWebSocketFrameHeader(const WebSocketFrameHeader& header,
                                  const WebSocketMaskingKey* masking_key,
                                  char* buffer,
                                  int buffer_size);
    
    WebSocketMaskingKey GenerateWebSocketMaskingKey();
    
    void MaskWebSocketFramePayload(const WebSocketMaskingKey& masking_key,
                                   uint64_t frame_offset,
                                   char* const data,
                                   int data_size);
}

#endif
