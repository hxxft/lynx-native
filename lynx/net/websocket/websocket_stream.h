#ifndef LYNX_NET_WEBSOCKET_WEBSOCKET_STREAM_H_
#define LYNX_NET_WEBSOCKET_WEBSOCKET_STREAM_H_

#include "base/task/callback.h"
#include "base/scoped_vector.h"
#include "net/websocket/websocket_frame.h"
#include "net/socket/socket.h"
#include "base/io_buffer.h"
#include "base/scoped_ptr.h"
#include "net/websocket/websocket_frame_parser.h"

namespace net {
    class WebSocketStream {
    public:
        WebSocketStream(Socket* socket);
        ~WebSocketStream();
        
        int ReadFrames(std::vector<base::ScopedPtr<WebSocketFrame>>* frames,
                       base::CompletionCallbackBase* callback);
        
        int WriteFrames(base::ScopedPtr<WebSocketFrame> frame,
                        base::CompletionCallbackBase* callback);
        
        void Close();
        
        Socket* socket() {
            return socket_.Get();
        }
    private:
        int WriteEverything(base::ScopedRefPtr<base::DrainableIOBuffer>& buffer, base::ScopedPtr<base::CompletionCallbackBase> callback);
        
        void OnWriteComplete(int result);
        
        
        // Attempts to parse the output of a read as WebSocket frames. On success,
        // returns OK and places the frame(s) in |frames|.
        int HandleReadResult(int result,
                             std::vector<base::ScopedPtr<WebSocketFrame>>* frames);
        
        // Converts the chunks in |frame_chunks| into frames and writes them to
        // |frames|. |frame_chunks| is destroyed in the process. Returns
        // ERR_WS_PROTOCOL_ERROR if an invalid chunk was found. If one or more frames
        // was added to |frames|, then returns OK, otherwise returns ERR_IO_PENDING.
        int ConvertChunksToFrames(
                                  std::vector<base::ScopedPtr<WebSocketFrameChunk>>* frame_chunks,
                                  std::vector<base::ScopedPtr<WebSocketFrame>>* frames);

        // Converts a |chunk| to a |frame|. |*frame| should be NULL on entry to this
        // method. If |chunk| is an incomplete control frame, or an empty middle
        // frame, then |*frame| may still be NULL on exit. If an invalid control frame
        // is found, returns ERR_WS_PROTOCOL_ERROR and the stream is no longer
        // usable. Otherwise returns OK (even if frame is still NULL).
        int ConvertChunkToFrame(base::ScopedPtr<WebSocketFrameChunk> chunk,
                                base::ScopedPtr<WebSocketFrame>* frame);

        // Creates a frame based on the value of |is_final_chunk|, |data| and
        // |current_frame_header_|. Clears |current_frame_header_| if |is_final_chunk|
        // is true. |data| may be NULL if the frame has an empty payload. A frame in
        // the middle of a message with no data is not useful; in this case the
        // returned frame will be NULL. Otherwise, |current_frame_header_->opcode| is
        // set to Continuation after use if it was Text or Binary, in accordance with
        // WebSocket RFC6455 section 5.4.
        base::ScopedPtr<WebSocketFrame> CreateFrame(
                                               bool is_final_chunk,
                                                    const base::ScopedRefPtr<base::IOBufferWithSize>& data);
        
        
        void AddToIncompleteControlFrameBody(const base::ScopedRefPtr<base::IOBufferWithSize>& data_buffer);
        
        void OnReadComplete(int result);
        
        base::ScopedPtr<Socket> socket_;
        
        base::ScopedRefPtr<base::IOBufferWithSize> read_buffer_;
        
        base::ScopedPtr<base::CompletionCallbackBase> write_callback_;
        
        base::ScopedRefPtr<base::DrainableIOBuffer> write_buffer_;
        std::vector<base::ScopedPtr<WebSocketFrame>>* read_frames_;
        
        base::ScopedPtr<base::CompletionCallbackBase> read_callback_;
        
        WebSocketFrameParser parser_;
        
        base::ScopedPtr<WebSocketFrameHeader>  current_frame_header_;
        
        base::ScopedRefPtr<base::GrowableIOBuffer> incomplete_control_frame_body_;
        
    };
}

#endif
