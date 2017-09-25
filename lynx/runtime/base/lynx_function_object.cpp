// Copyright 2017 The Lynx Authors. All rights reserved.

#include "lynx_function_object.h"
#include "lynx_object_template.h"
#include "runtime/base/lynx_value.h"

namespace jscore {

    LynxFunctionObject::LynxFunctionObject() {

    }

    LynxFunctionObject::~LynxFunctionObject() {

    }

    void LynxFunctionObject::RegisterMethodCallback(std::string method_name,
                                LynxMethodCallback callback) {
        methods_[method_name] = callback;
    }

    base::ScopedPtr<LynxValue>
    LynxFunctionObject::MethodCallback(std::string name,
                                   base::ScopedPtr<LynxArray> value) {
        auto it = methods_.find(name);
        if (it != methods_.end()) {
            LynxMethodCallback callback = it->second;
            return (*callback)(this, name, value);
        }
        return base::ScopedPtr<LynxValue>();
    }
}