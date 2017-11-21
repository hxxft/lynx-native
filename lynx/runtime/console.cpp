// Copyright 2017 The Lynx Authors. All rights reserved.

#include <runtime/base/lynx_value.h>
#include <runtime/jsc/jsc_helper.h>
#include "runtime/console.h"
#include "runtime/base/lynx_array.h"
#include "base/print.h"

namespace jscore {
    Console::Console() {
        set_class_name("Console");
        RegisterMethodCallback("log", &LogCallback);
        RegisterMethodCallback("warn", &LogCallback);
        RegisterMethodCallback("error", &LogCallback);
    }

    Console::~Console() {

    }

    base::ScopedPtr<LynxValue>
    Console::LogCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array) {
        if (array.Get() != NULL && array->Size() > 0) {
            if(array->Size() == 1) {
                LOGD("lynx-js-log", "%s", JSCHelper::ConvertToString(array->Get(0)).c_str());
            }
            else{
                LOGD("lynx-js-log", "%s", JSCHelper::ConvertToString(array.Get()).c_str());
            }

        }
        return base::ScopedPtr<LynxValue>(NULL);
    }
}