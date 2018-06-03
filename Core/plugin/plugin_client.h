// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PLUGIN_PLUGIN_CLIENT_H_
#define LYNX_PLUGIN_PLUGIN_CLIENT_H_

#include "base/scoped_ptr_map.h"
#include "plugin/base/plugin.h"
#include "runtime/base/lynx_object.h"
#include "runtime/runtime.h"

namespace plugin {

class PluginClient : public jscore::LynxObject, public lynx::EventTarget {
 public:
  PluginClient(jscore::JSContext* context);
  virtual ~PluginClient();

  base::ScopedPtr<jscore::LynxValue> Init(
      base::ScopedPtr<jscore::LynxArray>& array);
  base::ScopedPtr<jscore::LynxValue> Exec(
      base::ScopedPtr<jscore::LynxArray>& array);
  base::ScopedPtr<jscore::LynxValue> AddEventListener(
      base::ScopedPtr<jscore::LynxArray>& array);
  base::ScopedPtr<jscore::LynxValue> RemoveEventListener(
      base::ScopedPtr<jscore::LynxArray>& array);
  void Return(int method_id,
              bool successed,
              base::ScopedPtr<jscore::LynxArray> result);

  void RunOnClientThread(base::Closure* clousre);

 private:
  virtual void RegisterEvent(const std::string& plugin_event, RegisterEventType type) override;

  base::ScopedPtr<jscore::LynxFunction> callback_;

  jscore::Runtime* runtime_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PluginClient);
};
}  // namespace plugin
#endif  // LYNX_PLUGIN_PLUGIN_CLIENT_H_
