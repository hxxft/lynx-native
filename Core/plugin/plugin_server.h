// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PLUGIN_PLUGIN_SERVER_H_
#define LYNX_PLUGIN_PLUGIN_SERVER_H_

#include "base/scoped_ptr.h"
#include "runtime/base/lynx_array.h"

namespace plugin {
    class Plugin;
class PluginClient;
class PluginServer {
 public:
  static void Initialize();
  static void Exec(PluginClient* client,
                   base::ScopedPtr<jscore::LynxArray>& array);
  static void AddEvent(PluginClient* client,
                       const std::string& name,
                       const std::string& event);
  static void RemoveEvent(PluginClient* client,
                          const std::string& name,
                          const std::string& event);
    static void Return(long client_id, int method,
                       bool successed,
                       base::ScopedPtr<jscore::LynxArray>& array);
    
    static void DispatchEvent(const std::string& name,
                              const std::string& event,
                       base::ScopedPtr<jscore::LynxArray>& array);
  static void Register(PluginClient* client);
  static void UnRegister(PluginClient* client);
};
}  // namespace plugin

#endif  // LYNX_PLUGIN_PLUGIN_SERVER_H_
