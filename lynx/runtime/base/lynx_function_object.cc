// Copyright 2017 The Lynx Authors. All rights reserved.

#include "lynx_function_object.h"
#include "lynx_object_template.h"
#include "runtime/base/lynx_value.h"
#include "base/log/logging.h"

namespace jscore {

    LynxFunctionObject::LynxFunctionObject() {

    }

    LynxFunctionObject::~LynxFunctionObject() {

    }

    void LynxFunctionObject::RegisterMethodCallback(const std::string& method_name,
                                LynxMethodCallback callback) {
        methods_[method_name] = callback;
    }

    base::ScopedPtr<LynxValue>
    LynxFunctionObject::MethodCallback(const std::string& name,
                                   base::ScopedPtr<LynxArray> value) {
        auto it = methods_.find(name);
        if (it != methods_.end()) {
            LynxMethodCallback callback = it->second;
            return (*callback)(this, name, value);
        }else {
            DLOG(WARNING) << name << ": method not found!";
            return base::ScopedPtr<LynxValue>();
        }
    }
}
