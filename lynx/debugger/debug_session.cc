// Copyright 2017 The Lynx Authors. All rights reserved.

#include "debugger/debug_session.h"
#include "base/log/logging.h"

#include "third_party/jsoncpp/include/json/json.h"

namespace debug {

DebugSession::DebugSession(Dispatcher* dispatcher)
    : socket_(),
      dispatcher_(dispatcher),
      debug_thread_(nullptr),
      js_thread_(nullptr) {}

DebugSession::~DebugSession() {}

void DebugSession::Start(const std::string& address, int port) {
  if (socket_.Get() == nullptr) {
    socket_.Reset(
        new net::WebSocket(new DebugSocketListener(this), address, port));
  }
  socket_->Connect();
}

void DebugSession::Send(const std::string& command_type,
                        const std::string& content) {
  Json::Value message;
  message["fromType"] = "device";
  message["commandType"] = command_type;
  message["content"] = content;
  socket_->SendText(message.toStyledString());
}

void DebugSession::Dispatch(
    const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
    uint64_t size) {
  if (size > 0) {
    dispatcher_->DispatchMessage(data_buffer, size);
  }
}

void DebugSocketListener::OnOpen(
    const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
    uint64_t size) {
  DLOG(INFO) << "Debugger Connect Successed!";
}

void DebugSocketListener::OnMessage(
    const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
    uint64_t size) {
  std::string message(data_buffer->data(), size);
  if (delegate_) {
    delegate_->Dispatch(data_buffer, size);
  }
}

void DebugSocketListener::OnClosing() {}

void DebugSocketListener::OnClosed() {}

void DebugSocketListener::OnFailure() {
  DLOG(ERROR) << "Debugger connect Failed!";
}
}  // namespace debug
