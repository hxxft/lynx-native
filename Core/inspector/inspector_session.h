#ifndef LYNX_INSPECTOR_INSPECTOR_SESSION_H_
#define LYNX_INSPECTOR_INSPECTOR_SESSION_H_

#include "base/threading/thread.h"
#include "inspector/inspector_response.h"
#include "net/websocket/websocket.h"

#include <string>

namespace debug {

class InspectorSession {
 public:
  class Dispatcher {
   public:
    virtual void DispatchMessage(
        const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
        uint64_t size) = 0;
  };
  InspectorSession(Dispatcher* dispatcher);
  ~InspectorSession();

  void Start(const std::string& address, int port);

  void Dispatch(const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                uint64_t size);

  void Send(const std::string& command_type, const std::string& content);

  void Send(const std::string& command_type, Json::Value& content);

  void Close();

  void Send(const std::string& message);

 private:
  base::ScopedPtr<net::WebSocket> socket_;
  Dispatcher* dispatcher_;
  base::Thread* inspector_thread_;
  base::Thread* js_thread_;
  InspectorBasicInfo basic_info_;
};

class InspectorSocketListener : public net::WebSocket::Listener {
 public:
  InspectorSocketListener(InspectorSession* delegate) : delegate_(delegate) {}
  virtual ~InspectorSocketListener() {}
  virtual void OnOpen(const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                      uint64_t size);
  virtual void OnMessage(const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
                         uint64_t size);
  virtual void OnClosing();
  virtual void OnClosed();
  virtual void OnFailure();

 private:
  InspectorSession* delegate_;
};
}  // namespace debug

#endif
