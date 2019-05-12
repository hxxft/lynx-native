#include "inspector/inspector.h"
#include "base/task/callback.h"
#include "inspector/inspector_css_agent.h"
#include "inspector/inspector_dom_agent.h"

namespace {
const char DOMAIN_DOT = '.';
}

namespace debug {
Inspector::Inspector(base::Thread* js_thread, const std::string& name)
    : js_thread_(js_thread),
      inspector_thread_(
          lynx_new base::Thread(base::MessageLoop::MESSAGE_LOOP_IO, name)) {
  inspector_thread_->Start();
}

void Inspector::Attach(jscore::Runtime* runtime,
                       const std::string& address,
                       int port) {
  runtime_ = runtime;
  if (session_.Get() == nullptr) {
    agent_map_.add("DOM", new InspectorDOMAgent(runtime, this));
    agent_map_.add("CSS", new InspectorCSSAgent(runtime));
    session_.Reset(new InspectorSession(this));
  }
  base::ScopedRefPtr<Inspector> ref(this);
  inspector_thread_->Looper()->PostTask(
      base::Bind(&Inspector::AttachToInspectorThread, ref, address, port));
}

void Inspector::Send(Json::Value content) {
  base::ScopedRefPtr<Inspector> ref(this);
  std::string domain;
  inspector_thread_->Looper()->PostTask(
      base::Bind(&Inspector::SendToInspectorThread, ref, domain, content));
}

void Inspector::AttachToInspectorThread(const std::string& address, int port) {
  session_->Start(address, port);
}

void Inspector::SendToInspectorThread(const std::string& command_type,
                                      Json::Value content) {
  session_->Send(command_type, content);
}

void Inspector::DispatchMessageToJSThread(const std::string& message) {
  printf("recive from server: %s\n", message.c_str());
  Json::Value root;
  if (!reader_.parse(message, root, false)) {
    return;
  }

  std::string method = root["method"].asString();
  std::string domain = method.substr(0, method.find(DOMAIN_DOT));

  Json::Value content;
  auto iter = agent_map_.find(domain);
  if (iter == agent_map_.end()) {
    content["result"] = Json::Value(Json::objectValue);
    content["id"] = root["id"].asInt();
  } else {
    InspectorAgentBase* agent = iter->second;
    content = agent->CallMethod(root);
  }
  base::ScopedRefPtr<Inspector> ref(this);
  inspector_thread_->Looper()->PostTask(
      base::Bind(&Inspector::SendToInspectorThread, ref, domain, content));
}

void Inspector::DispatchMessage(
    const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
    uint64_t size) {
  base::ScopedRefPtr<Inspector> ref(this);
  std::string message(data_buffer->data(), size);
  js_thread_->Looper()->PostTask(
      base::Bind(&Inspector::DispatchMessageToJSThread, ref, message));
}

}  // namespace debug
