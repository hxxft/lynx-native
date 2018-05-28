
#include "net/websocket/websocket_frame.h"
#include "net/base/net_errors.h"
#include "net/websocket/websocket.h"
#include "base/big_endian.h"
#include "base/rand_util.h"

namespace net {
    namespace {
        const uint8_t kFinalBit = 0x80;
        const uint8_t kReserved1Bit = 0x40;
        const uint8_t kReserved2Bit = 0x20;
        const uint8_t kReserved3Bit = 0x10;
        const uint8_t kOpCodeMask = 0xF;
        const uint8_t kMaskBit = 0x80;
        const uint64_t kMaxPayloadLengthWithoutExtendedLengthField = 125;
        const uint64_t kPayloadLengthWithTwoByteExtendedLengthField = 126;
        const uint64_t kPayloadLengthWithEightByteExtendedLengthField = 127;
        
        inline void MaskWebSocketFramePayloadByBytes(
                                                     const WebSocketMaskingKey& masking_key,
                                                     size_t masking_key_offset,
                                                     char* const begin,
                                                     char* const end) {
            for (char* masked = begin; masked != end; ++masked) {
                *masked ^= masking_key.key_[masking_key_offset++];
                if (masking_key_offset == WebSocketFrameHeader::kMaskingKeyLength)
                    masking_key_offset = 0;
            }
        }
    }
    
    base::ScopedPtr<WebSocketFrameHeader> WebSocketFrameHeader::Clone() const {
        base::ScopedPtr<WebSocketFrameHeader> ret(new WebSocketFrameHeader(opcode_));
        ret->CopyFrom(*this);
        return ret;
    }
    
    void WebSocketFrameHeader::CopyFrom(const WebSocketFrameHeader& source) {
        final_ = source.final_;
        reserved1_ = source.reserved1_;
        reserved2_ = source.reserved2_;
        reserved3_ = source.reserved3_;
        opcode_ = source.opcode_;
        masked_ = source.masked_;
        payload_length_ = source.payload_length_;
    }
    
    int GetWebSocketFrameHeaderSize(const WebSocketFrameHeader& header) {
        int extended_length_size = 0;
        if (header.payload_length_ > kMaxPayloadLengthWithoutExtendedLengthField &&
            header.payload_length_ <= UINT16_MAX) {
            extended_length_size = 2;
        } else if (header.payload_length_ > UINT16_MAX) {
            extended_length_size = 8;
        }
        
        return (WebSocketFrameHeader::kBaseHeaderSize + extended_length_size +
                (header.masked_ ? WebSocketFrameHeader::kMaskingKeyLength : 0));
    }
    
    int WriteWebSocketFrameHeader(const WebSocketFrameHeader& header,
                                  const WebSocketMaskingKey* masking_key,
                                  char* buffer,
                                  int buffer_size) {
        
        int header_size = GetWebSocketFrameHeaderSize(header);
        if (header_size > buffer_size)
            return ERR_INVALID_ARGUMENT;
        
        int buffer_index = 0;
        
        uint8_t first_byte = 0u;
        first_byte |= header.final_ ? kFinalBit : 0u;
        first_byte |= header.reserved1_ ? kReserved1Bit : 0u;
        first_byte |= header.reserved2_ ? kReserved2Bit : 0u;
        first_byte |= header.reserved3_ ? kReserved3Bit : 0u;
        first_byte |= header.opcode_ & kOpCodeMask;
        buffer[buffer_index++] = first_byte;
        
        int extended_length_size = 0;
        uint8_t second_byte = 0u;
        second_byte |= header.masked_ ? kMaskBit : 0u;
        if (header.payload_length_ <= kMaxPayloadLengthWithoutExtendedLengthField) {
            second_byte |= header.payload_length_;
        } else if (header.payload_length_ <= UINT16_MAX) {
            second_byte |= kPayloadLengthWithTwoByteExtendedLengthField;
            extended_length_size = 2;
        } else {
            second_byte |= kPayloadLengthWithEightByteExtendedLengthField;
            extended_length_size = 8;
        }
        buffer[buffer_index++] = second_byte;
        
        // Writes "extended payload length" field.
        if (extended_length_size == 2) {
            uint16_t payload_length_16 = static_cast<uint16_t>(header.payload_length_);
            base::WriteBigEndian(buffer + buffer_index, payload_length_16);
            buffer_index += sizeof(payload_length_16);
        } else if (extended_length_size == 8) {
            base::WriteBigEndian(buffer + buffer_index, header.payload_length_);
            buffer_index += sizeof(header.payload_length_);
        }
        
        // Writes "masking key" field, if needed.
        if (header.masked_) {
            std::copy(masking_key->key_,
                      masking_key->key_ + WebSocketFrameHeader::kMaskingKeyLength,
                      buffer + buffer_index);
            buffer_index += WebSocketFrameHeader::kMaskingKeyLength;
        }
        return header_size;
    }
    
    WebSocketMaskingKey GenerateWebSocketMaskingKey() {
        WebSocketMaskingKey masking_key;
        base::RandomBuffer(masking_key.key_, WebSocketFrameHeader::kMaskingKeyLength);
        return masking_key;
    }
    
    void MaskWebSocketFramePayload(const WebSocketMaskingKey& masking_key,
                                   uint64_t frame_offset,
                                   char* const data,
                                   int data_size) {
        static const size_t kMaskingKeyLength =
        WebSocketFrameHeader::kMaskingKeyLength;
        

        size_t packed_mask_key;
        static const size_t kPackedMaskKeySize = sizeof(packed_mask_key);
        static_assert((kPackedMaskKeySize >= kMaskingKeyLength &&
                       kPackedMaskKeySize % kMaskingKeyLength == 0),
                      "PackedMaskType size is not a multiple of mask length");
        char* const end = data + data_size;
        // If the buffer is too small for the vectorised version to be useful, revert
        // to the byte-at-a-time implementation early.
        if (data_size <= static_cast<int>(kPackedMaskKeySize * 2)) {
            MaskWebSocketFramePayloadByBytes(
                                             masking_key, frame_offset % kMaskingKeyLength, data, end);
            return;
        }
        const size_t data_modulus =
        reinterpret_cast<size_t>(data) % kPackedMaskKeySize;
        char* const aligned_begin =
        data_modulus == 0 ? data : (data + kPackedMaskKeySize - data_modulus);
        // Guaranteed by the above check for small data_size.

        MaskWebSocketFramePayloadByBytes(
                                         masking_key, frame_offset % kMaskingKeyLength, data, aligned_begin);
        const size_t end_modulus = reinterpret_cast<size_t>(end) % kPackedMaskKeySize;
        char* const aligned_end = end - end_modulus;
        // Guaranteed by the above check for small data_size.

        // Create a version of the mask which is rotated by the appropriate offset
        // for our alignment. The "trick" here is that 0 XORed with the mask will
        // give the value of the mask for the appropriate byte.
        char realigned_mask[kMaskingKeyLength] = {};
        MaskWebSocketFramePayloadByBytes(
                                         masking_key,
                                         (frame_offset + aligned_begin - data) % kMaskingKeyLength,
                                         realigned_mask,
                                         realigned_mask + kMaskingKeyLength);
        
        for (size_t i = 0; i < kPackedMaskKeySize; i += kMaskingKeyLength) {
            // memcpy() is allegedly blessed by the C++ standard for type-punning.
            memcpy(reinterpret_cast<char*>(&packed_mask_key) + i,
                   realigned_mask,
                   kMaskingKeyLength);
        }
        
        // The main loop.
        for (char* merged = aligned_begin; merged != aligned_end;
             merged += kPackedMaskKeySize) {
            // This is not quite standard-compliant C++. However, the standard-compliant
            // equivalent (using memcpy()) compiles to slower code using g++. In
            // practice, this will work for the compilers and architectures currently
            // supported by Chromium, and the tests are extremely unlikely to pass if a
            // future compiler/architecture breaks it.
            *reinterpret_cast<size_t*>(merged) ^= packed_mask_key;
        }
        
        MaskWebSocketFramePayloadByBytes(
                                         masking_key,
                                         (frame_offset + (aligned_end - data)) % kMaskingKeyLength,
                                         aligned_end,
                                         end);
    }
}
