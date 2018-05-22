#include "plugin/base/plugin.h"
#include "plugin/base/plugin_manager.h"

namespace plugin {
    void Plugin::Return(int method_id, ResultType type, base::ScopedPtr<jscore::LynxArray> args){
        manager_->Return(method_id, type, args);
    }
}

