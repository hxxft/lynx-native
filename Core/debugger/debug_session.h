// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_DEBUGGER_DEBUG_SESSION_H_
#define LYNX_DEBUGGER_DEBUG_SESSION_H_

#include "base/threading/thread.h"
#include "net/websocket/websocket.h"

namespace debug {
class DebugSession {
 public:
  class Dispatcher {
   public:
    virtual ~Dispatcher() {}
    virtual void DispatchMessage(
        const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
        uint64_t size) = 0;
  };
  DebugSession(Dispatcher* dispatcher);
  ~DebugSession();

  void Start(const std::string& address, int port);

  void Dispatch(const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                uint64_t size);

  void Send(const std::string& command_type, const std::string& content);

  void Close();

 private:
  base::ScopedPtr<net::WebSocket> socket_;
  Dispatcher* dispatcher_;
  base::Thread* debug_thread_;
  base::Thread* js_thread_;
};

class DebugSocketListener : public net::WebSocket::Listener {
 public:
  DebugSocketListener(DebugSession* delegate) : delegate_(delegate) {}
  virtual ~DebugSocketListener() {}
  virtual void OnOpen(const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                      uint64_t size);
  virtual void OnMessage(const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                         uint64_t size);
  virtual void OnClosing();
  virtual void OnClosed();
  virtual void OnFailure();

 private:
  DebugSession* delegate_;
};
}  // namespace debug

#endif  // LYNX_DEBUGGER_DEBUG_SESSION_H_
