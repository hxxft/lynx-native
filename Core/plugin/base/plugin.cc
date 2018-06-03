// Copyright 2017 The Lynx Authors. All rights reserved.

#include "plugin/base/plugin.h"
#include "plugin/plugin_server.h"

namespace plugin {
    void Plugin::Return(long client, int method, bool successed, base::ScopedPtr<jscore::LynxArray> args) {
        PluginServer::Return(client, method, successed, args);
    }
    
    void Plugin::DispatchEvent(const std::string& name, const std::string& event, base::ScopedPtr<jscore::LynxArray> args) {
        PluginServer::DispatchEvent(name, event, args);
    }
}

