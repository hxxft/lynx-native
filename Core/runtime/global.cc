// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/global.h"

#include "runtime/timed_task.h"
#include "runtime/console.h"
#include "runtime/navigator.h"
#include "runtime/screen.h"
#include "runtime/loader.h"
#include "runtime/document.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/js/defines.h"
#include "runtime/js/js_context.h"
#include "plugin/base/plugin_manager.h"

namespace jscore {

    #define FOR_EACH_METHOD_BINDING(V)      \
        V(Global, SetTimeout)               \
        V(Global, SetInterval)              \
        V(Global, ClearTimeout)             \
        V(Global, ClearInterval)

    #define FOR_EACH_FIELD_GET_BINDING(V)   \
        V(Global, DevicePixelRatio)         \
        V(Global, Console)                  \
        V(Global, Navigator)                \
        V(Global, Screen)                   \
        V(Global, Loader)                   \
        V(Global, Document)                 \
        V(Global, Plugin)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)
    FOR_EACH_FIELD_GET_BINDING(DEFINE_GET_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Global)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
        FOR_EACH_FIELD_GET_BINDING(REGISTER_GET_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    Global::Global(JSContext *context) : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)),
                                         console_(NULL),
                                         navigator_(NULL),
                                         screen_(NULL),
                                         loader_(NULL),
                                         document_(NULL) {
    }

    Global::~Global() {
        console_->UnprotectJSObject();
        navigator_->UnprotectJSObject();
        screen_->UnprotectJSObject();
        loader_->UnprotectJSObject();
        document_->UnprotectJSObject();
    }

    void Global::OnJSObjectAttached() {
        console_ = lynx_new Console(context_);
        console_->ProtectJSObject();
        navigator_ = lynx_new Navigator(context_);
        navigator_->ProtectJSObject();
        screen_ = lynx_new Screen(context_);
        screen_->ProtectJSObject();
        loader_ = lynx_new Loader(context_);
        loader_->ProtectJSObject();
        document_ = lynx_new Document(context_);
        document_->ProtectJSObject();
        
        plugin_ = lynx_new plugin::PluginManager(context_);
        plugin_->ProtectJSObject();
    }

    base::ScopedPtr<LynxValue> Global::SetTimeout(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() >= 2
             && array->Get(1)->type_ == LynxValue::Type::VALUE_INT
             && array->Get(0)->type_ == LynxValue::Type::VALUE_LYNX_FUNCTION) {
            base::ScopedPtr<TimedTaskInvoker> invoker(lynx_new TimedTaskInvoker());
            invoker->SetTimeout(context_,
                                array->Get(0)->data_.lynx_function,
                                array->Get(1)->data_.i);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Global::SetInterval(base::ScopedPtr<LynxArray> &array) {
        if (array->Size() >= 2
            && array->Get(1)->type_ == LynxValue::Type::VALUE_INT
            && array->Get(0)->type_ == LynxValue::Type::VALUE_LYNX_FUNCTION) {
            base::ScopedPtr<TimedTaskInvoker> invoker(lynx_new TimedTaskInvoker());
            invoker->SetInterval(context_,
                                 array->Get(0)->data_.lynx_function,
                                 array->Get(1)->data_.i);
        }
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Global::ClearTimeout(base::ScopedPtr<LynxArray> &array) {
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Global::ClearInterval(base::ScopedPtr<LynxArray> &array) {
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue> Global::GetDevicePixelRatio() {
        int density = (int) config::GlobalConfigData::GetInstance()->screen_density();
        return LynxValue::MakeInt(density);
    }

    base::ScopedPtr<LynxValue> Global::GetConsole() {
        return LynxValue::MakeObject(console());
    }

    base::ScopedPtr<LynxValue> Global::GetNavigator() {
        return LynxValue::MakeObject(navigator());
    }

    base::ScopedPtr<LynxValue> Global::GetScreen() {
        return LynxValue::MakeObject(screen());
    }

    base::ScopedPtr<LynxValue> Global::GetLoader() {
        return LynxValue::MakeObject(loader());
    }

    base::ScopedPtr<LynxValue> Global::GetDocument() {
        return LynxValue::MakeObject(document());
    }

    base::ScopedPtr<LynxValue> Global::GetPlugin() {
        return LynxValue::MakeObject(plugin());
    }
}

