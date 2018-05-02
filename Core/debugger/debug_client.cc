// Copyright 2017 The Lynx Authors. All rights reserved.

#include "debugger/debug_client.h"

#include "base/task/callback.h"

#include "runtime/runtime.h"

namespace {
static debug::DebugClient* g_debugger_instance_ = nullptr;
static std::map<std::string, debug::DebugType> g_debug_types_ = {
    {"reload", debug::Debug_Reload}};
}  // namespace

namespace debug {

DebugClient* DebugClient::Debugger() {
  if (g_debugger_instance_ == nullptr) {
    g_debugger_instance_ = lynx_new DebugClient;
  }
  return g_debugger_instance_;
}

DebugClient::DebugClient() {
  AddRef();
}

void DebugClient::Initialize(DebugHost* host) {
  host_.Reset(host);
  if (ui_thread_.Get() == nullptr) {
    ui_thread_.Reset(lynx_new base::Thread(base::MessageLoop::MESSAGE_LOOP_UI));
  }
  if (debug_thread_.Get() == nullptr) {
    debug_thread_.Reset(lynx_new base::Thread(
        base::MessageLoop::MESSAGE_LOOP_IO, "DebugThread"));
    Start();
  }
}

void DebugClient::Start(const std::string& address, int port) {
  debug_thread_->Start();
  if (session_.Get() == nullptr) {
    session_.Reset(new DebugSession(this));
  }
  base::ScopedRefPtr<DebugClient> ref(this);
  debug_thread_->Looper()->PostTask(
      base::Bind(&DebugClient::StartOnDebugThread, ref, address, port));
}

void DebugClient::StartOnDebugThread(const std::string& address, int port) {
  session_->Start(address, port);
}

void DebugClient::Send(const std::string& command_type,
                       const std::string& content) {
  if (debug_thread_.Get()) {
    base::ScopedRefPtr<DebugClient> ref(this);
    debug_thread_->Looper()->PostTask(base::Bind(
        &DebugClient::SendToDebugThread, ref, command_type, content));
  }
}

void DebugClient::SendToDebugThread(const std::string& command_type,
                                    const std::string& content) {
  session_->Send(command_type, content);
}

void DebugClient::DispatchToUIThread(DebugType type) {
  host_->Run(type);
}

void DebugClient::DispatchMessage(
    const base::ScopedRefPtr<base::IOBuffer>& data_buffer,
    uint64_t size) {
  if (ui_thread_.Get() == nullptr)
    return;
  base::ScopedRefPtr<DebugClient> ref(this);
  std::string message(data_buffer->data(), size);
  Json::Value content;
  Json::Reader reader;
  reader.parse(message, content, false);
  auto iter = g_debug_types_.find(content["content"]["method"].asString());
  if (iter != g_debug_types_.end()) {
    ui_thread_->Looper()->PostTask(
        base::Bind(&DebugClient::DispatchToUIThread, ref, iter->second));
  }
}

}  // namespace debug
