// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/console.h"

#include "base/log/logging.h"
#include "debugger/debug_client.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/js/class_template.h"
#include "runtime/js/js_context.h"

namespace jscore {

    #define FOR_EACH_METHOD_BINDING(V)    \
        V(Console, Log)                   \
        V(Console, Warn)                  \
        V(Console, Error)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Console)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    Console::Console(JSContext* context) : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)) {
    }

    Console::~Console() {

    }

    base::ScopedPtr<LynxValue> Console::Error(base::ScopedPtr<LynxArray>& array) {
        return Log(array);
    }

    base::ScopedPtr<LynxValue> Console::Warn(base::ScopedPtr<LynxArray>& array) {
        return Log(array);
    }

    base::ScopedPtr<LynxValue> Console::Log(base::ScopedPtr<LynxArray>& array) {
        std::string log;
        if (array.Get() != NULL && array->Size() > 0) {
            if(array->Size() == 1) {
                log = array->Get(0)->ToString();
            } else {
                log = array.Get()->ToString();
            }
            DLOG(LOG) << log;
        }
        debug::DebugClient::Debugger()->Send("log", log);
        return base::ScopedPtr<LynxValue>(NULL);
    }

}