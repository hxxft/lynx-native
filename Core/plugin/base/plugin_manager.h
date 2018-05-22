#ifndef LYNX_PLUGIN_PLUGIN_MANAGER_H_
#define LYNX_PLUGIN_PLUGIN_MANAGER_H_

#include "runtime/base/lynx_object.h"
#include "runtime/runtime.h"
#include "base/scoped_ptr_map.h"
#include "plugin/base/plugin.h"

namespace plugin {

    class PluginManager : public jscore::LynxObject {
        public:
        PluginManager(jscore::JSContext* context);
        virtual ~PluginManager();
        
        base::ScopedPtr<jscore::LynxValue> Init(base::ScopedPtr<jscore::LynxArray>& array);
        base::ScopedPtr<jscore::LynxValue> Exec(base::ScopedPtr<jscore::LynxArray>& array);
        void Return(int method_id, Plugin::ResultType type, base::ScopedPtr<jscore::LynxArray>& result);
        void Register(const char* name, Plugin*  plugin);
        private:
        void ReturnOnJSThread(int method_id, Plugin::ResultType type, base::ScopedPtr<jscore::LynxArray> result);
        
        typedef base::ScopedPtrMap<std::string, Plugin> Plugins;
        Plugins plugins_;
        
        base::ScopedPtr<jscore::LynxFunction> callback_;
        
        jscore::JSContext* context_;
        private:
        DISALLOW_COPY_AND_ASSIGN(PluginManager);
    };
}
#endif
