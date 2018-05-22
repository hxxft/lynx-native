#ifndef LYNX_PLUGIN_PLUGIN_H_
#define LYNX_PLUGIN_PLUGIN_H_

#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"

namespace plugin {
    class PluginManager;
    class Plugin {
    public:
        enum ResultType {
            ResultType_Fail,
            ResultType_Success,
            ResultType_Event
        };
        
        Plugin(PluginManager* manager):manager_(manager){}
        virtual ~Plugin(){}
        virtual void Exec(base::ScopedPtr<jscore::LynxArray> args){}
        void AddEventListener(base::ScopedPtr<jscore::LynxArray> args){}
        void Return(int method_id, ResultType type, base::ScopedPtr<jscore::LynxArray> args);
    private:
        PluginManager* manager_;
    };
}

#endif
