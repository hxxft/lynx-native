#include "net/websocket/websocket_stream.h"
#include "net/base/net_errors.h"

#include <limits> 

namespace net {
    namespace {
        const int kReadBufferSize = 32 * 1024;
        const uint64_t kMaxControlFramePayload = 125;
        
        int CalculateSerializedSizeAndTurnOnMaskBit(const base::ScopedPtr<WebSocketFrame>& frame) {
            const uint64_t kMaximumTotalSize = std::numeric_limits<int>::max();
            
            uint64_t total_size = 0;
            
            frame->header_.masked_ = true;
            uint64_t frame_size = frame->header_.payload_length_ +
            GetWebSocketFrameHeaderSize(frame->header_);
            total_size += frame_size;
            
            return static_cast<int>(total_size);
        }
    }
    
    WebSocketStream::WebSocketStream(Socket* socket):socket_(socket), read_buffer_(new base::IOBufferWithSize(kReadBufferSize)){
        
    }
    
    WebSocketStream::~WebSocketStream(){
        
    }
    
    int WebSocketStream::ReadFrames(std::vector<base::ScopedPtr<WebSocketFrame>>* frames,
                   base::CompletionCallbackBase* callback) {
        while (true) {
            int result = socket()->Read(read_buffer_.Get(),
                                        read_buffer_->size(),
                                        new base::CompletionCallback<WebSocketStream>(&WebSocketStream::OnReadComplete,
                                                                this));
            if (result == ERR_IO_PENDING) {
                read_callback_.Reset(callback);
                read_frames_ = frames;
                return result;
            }
            result = HandleReadResult(result, frames);
            if (result != ERR_IO_PENDING)
                return result;
        }
        return OK;
    }
    
    int WebSocketStream::WriteFrames(base::ScopedPtr<WebSocketFrame> frame,
                    base::CompletionCallbackBase* callback) {
        int total_size = CalculateSerializedSizeAndTurnOnMaskBit(frame);
        base::ScopedRefPtr<base::IOBufferWithSize> combined_buffer(new base::IOBufferWithSize(total_size));
        char* dest = combined_buffer->data();
        int remaining_size = total_size;
    
        WebSocketMaskingKey mask = GenerateWebSocketMaskingKey();
        int result = WriteWebSocketFrameHeader(frame->header_, &mask, dest, remaining_size);
        dest += result;
        remaining_size -= result;
        
        const int frame_size = static_cast<int>(frame->header_.payload_length_);
        
        if(frame_size > 0) {
            const char* const frame_data = frame->data_->data();
            std::copy(frame_data, frame_data + frame_size, dest);
            MaskWebSocketFramePayload(mask, 0, dest, frame_size);
            dest += frame_size;
            remaining_size -= frame_size;
        }
        
        base::ScopedRefPtr<base::DrainableIOBuffer> drainable_buffer(
                                                                     new base::DrainableIOBuffer(combined_buffer.Get(), total_size));
        return WriteEverything(drainable_buffer, base::ScopedPtr<base::CompletionCallbackBase>(callback));
    }
    
    void WebSocketStream::Close() {
        
    }
    
    int WebSocketStream::WriteEverything(
                                         base::ScopedRefPtr<base::DrainableIOBuffer>& buffer, base::ScopedPtr<base::CompletionCallbackBase> callback) {
        while (buffer->BytesRemaining() > 0) {
            int result = socket_->Write(
                                        buffer.Get(),
                                        buffer->BytesRemaining(),
                                        new base::CompletionCallback<WebSocketStream>(&WebSocketStream::OnWriteComplete, this));
            if (result > 0) {
                buffer->DidConsume(result);
            } else {
                write_callback_ = callback;
                write_buffer_ = buffer;
                return result;
            }
        }
        return OK;
    }
    
    void WebSocketStream::OnWriteComplete(int result) {
        if (result <= 0) {
            write_callback_->Run(result);
            return;
        }
        
        write_buffer_->DidConsume(result);
        result = WriteEverything(write_buffer_, write_callback_);
    }
    
    void WebSocketStream::OnReadComplete(int result) {
        result = HandleReadResult(result, read_frames_);
        if (result == ERR_IO_PENDING)
            result = ReadFrames(read_frames_, read_callback_.Release());
        if (result != ERR_IO_PENDING)
            read_callback_->Run(result);
    }
    
    int WebSocketStream::HandleReadResult(int result,
                                          std::vector<base::ScopedPtr<WebSocketFrame>>* frames) {
        if (result < 0)
            return result;
        if (result == 0)
            return ERR_CONNECTION_CLOSED;
        std::vector<base::ScopedPtr<WebSocketFrameChunk>> frame_chunks;
        if (!parser_.Decode(read_buffer_->data(), result, &frame_chunks))
            return WebSocketErrorToNetError(parser_.websocket_error());
        if (frame_chunks.empty())
            return ERR_IO_PENDING;
        return ConvertChunksToFrames(&frame_chunks, frames);
    }
    
    int WebSocketStream::ConvertChunksToFrames(
                                                    std::vector<base::ScopedPtr<WebSocketFrameChunk>>* frame_chunks,
                                                    std::vector<base::ScopedPtr<WebSocketFrame>>* frames) {
        for (size_t i = 0; i < frame_chunks->size(); ++i) {
            base::ScopedPtr<WebSocketFrame> frame;
            int result = ConvertChunkToFrame(std::move((*frame_chunks)[i]), &frame);
            if (result != OK)
                return result;
            if (frame.Get())
                frames->push_back(std::move(frame));
        }
        frame_chunks->clear();
        if (frames->empty())
            return ERR_IO_PENDING;
        return OK;
    }
    
    int WebSocketStream::ConvertChunkToFrame(base::ScopedPtr<WebSocketFrameChunk> chunk,
                                                  base::ScopedPtr<WebSocketFrame>* frame) {
        bool is_first_chunk = false;
        if (chunk->header.Get()) {
            is_first_chunk = true;
            current_frame_header_.Swap(chunk->header);
        }
        const int chunk_size = chunk->data.Get() ? chunk->data->size() : 0;
        base::ScopedRefPtr<base::IOBufferWithSize> data_buffer;
        data_buffer.Swap(chunk->data);
        const bool is_final_chunk = chunk->final_chunk;
        const WebSocketFrameHeader::OpCode opcode = current_frame_header_->opcode_;
        if (WebSocketFrameHeader::IsKnownControlOpCode(opcode)) {
            bool protocol_error = false;
            if (!current_frame_header_->final_) {
                protocol_error = true;
            }
            if (current_frame_header_->payload_length_ > kMaxControlFramePayload) {
                protocol_error = true;
            }
            if (protocol_error) {
                current_frame_header_.Reset();
                return ERR_WS_PROTOCOL_ERROR;
            }
            if (!is_final_chunk) {
                if (incomplete_control_frame_body_.Get()) {
                    AddToIncompleteControlFrameBody(data_buffer);
                } else {
                    incomplete_control_frame_body_ = new base::GrowableIOBuffer();
                    // This method checks for oversize control frames above, so as long as
                    // the frame parser is working correctly, this won't overflow. If a bug
                    // does cause it to overflow, it will CHECK() in
                    // AddToIncompleteControlFrameBody() without writing outside the buffer.
                    incomplete_control_frame_body_->SetCapacity(kMaxControlFramePayload);
                    AddToIncompleteControlFrameBody(data_buffer);
                }
                return OK;
            }
            if (incomplete_control_frame_body_.Get()) {
                AddToIncompleteControlFrameBody(data_buffer);
                const int body_size = incomplete_control_frame_body_->offset();
                base::ScopedRefPtr<base::IOBufferWithSize> body(new base::IOBufferWithSize(body_size));
                memcpy(body->data(),
                       incomplete_control_frame_body_->StartOfBuffer(),
                       body_size);
                incomplete_control_frame_body_ = NULL;  // Frame now complete.
                *frame = CreateFrame(is_final_chunk, body);
                return OK;
            }
        }
        

        // Convert the chunk to a complete frame.
        *frame = CreateFrame(is_final_chunk, data_buffer);
        return OK;
    }
    
    base::ScopedPtr<WebSocketFrame> WebSocketStream::CreateFrame(
                                                                 bool is_final_chunk,
                                                                 const base::ScopedRefPtr<base::IOBufferWithSize>& data) {
        base::ScopedPtr<WebSocketFrame> result_frame;
        const bool is_final_chunk_in_message =
        is_final_chunk && current_frame_header_->final_;
        const int data_size = data.Get() ? data->size() : 0;
        const WebSocketFrameHeader::OpCode opcode = current_frame_header_->opcode_;
        // Empty frames convey no useful information unless they are the first frame
        // (containing the type and flags) or have the "final" bit set.
        if (is_final_chunk_in_message || data_size > 0 ||
            current_frame_header_->opcode_ !=
            WebSocketFrameHeader::kOpCodeContinuation) {
            result_frame.Reset(new WebSocketFrame(opcode));
            result_frame->header_.CopyFrom(*(current_frame_header_.Get()));
            result_frame->header_.final_ = is_final_chunk_in_message;
            result_frame->header_.payload_length_ = data_size;
            result_frame->data_ = data.Get();
            // Ensure that opcodes Text and Binary are only used for the first frame in
            // the message. Also clear the reserved bits.
            // TODO(ricea): If a future extension requires the reserved bits to be
            // retained on continuation frames, make this behaviour conditional on a
            // flag set at construction time.
            if (!is_final_chunk && WebSocketFrameHeader::IsKnownDataOpCode(opcode)) {
                current_frame_header_->opcode_ = WebSocketFrameHeader::kOpCodeContinuation;
                current_frame_header_->reserved1_ = false;
                current_frame_header_->reserved2_ = false;
                current_frame_header_->reserved3_ = false;
            }
        }
        // Make sure that a frame header is not applied to any chunks that do not
        // belong to it.
        if (is_final_chunk)
            current_frame_header_.Reset();
        return result_frame;
    }
    
    void WebSocketStream::AddToIncompleteControlFrameBody(const base::ScopedRefPtr<base::IOBufferWithSize>& data_buffer) {
        if (!data_buffer.Get())
            return;
        const int new_offset =
        incomplete_control_frame_body_->offset() + data_buffer->size();
        memcpy(incomplete_control_frame_body_->data(),
               data_buffer->data(),
               data_buffer->size());
        incomplete_control_frame_body_->set_offset(new_offset);
    }
}
