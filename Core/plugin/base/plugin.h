// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_PLUGIN_PLUGIN_H_
#define LYNX_PLUGIN_PLUGIN_H_

#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"

#include "render/event_target.h"

#include <unordered_set>
#include <string>

namespace plugin {
    class PluginManager;
    class Plugin {
    public:
        
        Plugin(){}
        virtual ~Plugin(){}
        virtual void Exec(long client, base::ScopedPtr<jscore::LynxArray> args){}
        void Return(long client_id, int method_id, bool successed, base::ScopedPtr<jscore::LynxArray> args);

        virtual void AddEvent(const std::string& event) {}
        virtual void RemoveEvent(const std::string& event) {}
        void DispatchEvent(const std::string& name, const std::string& event, base::ScopedPtr<jscore::LynxArray> args);
        
        static Plugin* Create(const std::string& name);
    protected:
        std::unordered_set<std::string> register_events_;
    };
}

#endif
