// Copyright 2017 The Lynx Authors. All rights reserved.

#include "plugin/plugin_client.h"
#include "plugin/plugin_server.h"

#include "plugin/base/plugin.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_value.h"
#include "runtime/js/class_template.h"

#include "base/log/logging.h"

namespace plugin {

#define FOR_EACH_METHOD_BINDING(V)  \
  V(PluginClient, Exec)             \
  V(PluginClient, Init)             \
  V(PluginClient, AddEventListener) \
  V(PluginClient, RemoveEventListener)

// Defines methods and fields
FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)

// Defines default ClassTemplate
DEFINE_CLASS_TEMPLATE_START(PluginClient)
EXPOSE_CONSTRUCTOR(true)
FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
DEFINE_CLASS_TEMPLATE_END

PluginClient::PluginClient(jscore::JSContext* context)
    : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)),
      runtime_(context->runtime()) {}

PluginClient::~PluginClient() {}

base::ScopedPtr<jscore::LynxValue> PluginClient::Init(
    base::ScopedPtr<jscore::LynxArray>& array) {
  callback_.Reset(array->Get(0)->data_.lynx_function);
  return base::ScopedPtr<jscore::LynxValue>(NULL);
}

base::ScopedPtr<jscore::LynxValue> PluginClient::Exec(
    base::ScopedPtr<jscore::LynxArray>& array) {
  PluginServer::Exec(this, array);
  return base::ScopedPtr<jscore::LynxValue>(NULL);
}

base::ScopedPtr<jscore::LynxValue> PluginClient::AddEventListener(
    base::ScopedPtr<jscore::LynxArray>& array) {
  std::string name(array->Get(0)->data_.str);
  std::string event = array->Get(1)->data_.str;
  jscore::LynxFunction* js_function = array->Get(2)->data_.lynx_function;
  lynx::EventTarget::AddEventListener(name + "#" + event, js_function, false);
  return base::ScopedPtr<jscore::LynxValue>(NULL);
}

base::ScopedPtr<jscore::LynxValue> PluginClient::RemoveEventListener(
    base::ScopedPtr<jscore::LynxArray>& array) {
  std::string name(array->Get(0)->data_.str);
  std::string event = array->Get(1)->data_.str;
  jscore::LynxFunction* js_function = array->Get(2)->data_.lynx_function;
  lynx::EventTarget::RemoveEventListener(name + "#" + event, js_function);
  return base::ScopedPtr<jscore::LynxValue>(NULL);
}

void PluginClient::RegisterEvent(const std::string& plugin_event,
                                 RegisterEventType type) {
  size_t pos = plugin_event.find('#');
  std::string name = plugin_event.substr(0, pos);
  std::string event = plugin_event.substr(pos + 1,plugin_event.length() - pos);
  switch (type) {
    case EVENT_ADD:
      PluginServer::AddEvent(this, name, event);
      break;
    case EVENT_REMOVE:
      PluginServer::RemoveEvent(this, name, event);
      break;
    default:
      break;
  }
}

void PluginClient::Return(int method_id,
                          bool successed,
                          base::ScopedPtr<jscore::LynxArray> result) {
  base::ScopedPtr<jscore::LynxArray> args(lynx_new jscore::LynxArray);
  args->Push(jscore::LynxValue::MakeInt(method_id).Release());
  args->Push(jscore::LynxValue::MakeBool(successed).Release());
  args->Push(result.Release());
  callback_->Run(this, args.Get());
}

void PluginClient::RunOnClientThread(base::Closure* clousre) {
  runtime_->thread_manager()->RunOnJSThread(clousre);
}
}  // namespace plugin
