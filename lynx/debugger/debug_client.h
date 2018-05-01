// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_DEBUGGER_DEBUG_CLEINT_H_
#define LYNX_DEBUGGER_DEBUG_CLEINT_H_

#include "debugger/debug_client.h"

#include <string>

#include "debugger/debug_host.h"
#include "debugger/debug_session.h"
#include "third_party/jsoncpp/include/json/json.h"

namespace jscore {
class Runtime;
}

namespace debug {
class DebugClient : public DebugSession::Dispatcher,
                    public base::RefCountPtr<DebugClient> {
 public:
  static DebugClient* Debugger();
  DebugClient();

  void Send(const std::string& command_type, const std::string& content);

  void Initialize(DebugHost* host);

  virtual void DispatchMessage(
      const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
      uint64_t size);

 private:
  void Start(const std::string& address = "127.0.0.1", int port = 9527);
  void StartOnDebugThread(const std::string& address, int port);
  void SendToDebugThread(const std::string& command_type,
                         const std::string& content);
  void DispatchToUIThread(DebugType type);

  Json::Reader reader_;

  base::ScopedPtr<base::Thread> ui_thread_;
  base::ScopedPtr<base::Thread> debug_thread_;
  base::ScopedPtr<DebugSession> session_;
  base::ScopedPtr<DebugHost> host_;
};
}  // namespace debug

#endif  // LYNX_DEBUGGER_DEBUG_CLEINT_H_
