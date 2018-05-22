#include "plugin/base/plugin_manager.h"

#include "plugin/base/plugin.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/js/class_template.h"

#include "base/log/logging.h"

#include "plugin/base/plugin_builder.h"

namespace plugin {
    
    #define FOR_EACH_METHOD_BINDING(V)      \
    V(PluginManager, Exec)                  \
    V(PluginManager, Init)                  \

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)
    
    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(PluginManager)
    EXPOSE_CONSTRUCTOR(true)
    FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    PluginManager::PluginManager(jscore::JSContext* context) : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)), context_(context){

    }
    
    PluginManager::~PluginManager() {

    }
    
    void PluginManager::Register(const char* name, Plugin*  plugin) {
        plugins_.add(name, plugin);
    }
    
    base::ScopedPtr<jscore::LynxValue> PluginManager::Init(base::ScopedPtr<jscore::LynxArray>& array) {
        PluginBuilder::build(this);
        callback_.Reset(array->Get(0)->data_.lynx_function);
        return base::ScopedPtr<jscore::LynxValue>(NULL);
    }
    
    base::ScopedPtr<jscore::LynxValue> PluginManager::Exec(base::ScopedPtr<jscore::LynxArray>& array) {
        std::string module_name(array->Get(0)->data_.str);
        Plugins::iterator iter = plugins_.find(module_name);
        if(iter == plugins_.end()) {
            DLOG(ERROR) << "";
            return base::ScopedPtr<jscore::LynxValue>(NULL);
        }
        iter->second->Exec(array);
        return base::ScopedPtr<jscore::LynxValue>(NULL);
    }
    
    void PluginManager::Return(int method_id, Plugin::ResultType type, base::ScopedPtr<jscore::LynxArray>& result){
        if(context_) {
            context_->runtime()->thread_manager()->RunOnJSThread(base::Bind(&PluginManager::ReturnOnJSThread,
                                                                            base::ScopedRefPtr<PluginManager>(this), method_id, type, result));
        }
    }
    
    void PluginManager::ReturnOnJSThread(int method_id, Plugin::ResultType type, base::ScopedPtr<jscore::LynxArray> result) {
        base::ScopedPtr<jscore::LynxArray> args(lynx_new jscore::LynxArray);
        args->Push(jscore::LynxValue::MakeInt(method_id).Release());
        args->Push(jscore::LynxValue::MakeInt(type).Release());
        args->Push(result.Release());
        callback_->Run(this, args.Get());
    }
}
