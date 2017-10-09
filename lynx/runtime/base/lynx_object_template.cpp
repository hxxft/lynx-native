// Copyright 2017 The Lynx Authors. All rights reserved.

#include "JavaScriptCore/JavaScript.h"
#include "lynx_object_template.h"

#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_value.h"
#include "runtime/jsc/objects/object_template.h"

namespace jscore {

    LynxObjectTemplate::LynxObjectTemplate() : object_template_(NULL) {
    }

    LynxObjectTemplate::~LynxObjectTemplate() {

    }

    void LynxObjectTemplate::RegisterMethodCallback(const std::string& method_name,
                                                LynxMethodCallback callback) {
        methods_[method_name] = callback;
    }

    void LynxObjectTemplate::RegisterRawMethodCallback(const std::string& method_name,
                                                       JSObjectCallAsFunctionCallback callback) {
        raw_methods_[method_name] = callback;
    }

    void LynxObjectTemplate::RegisterAccessorCallback(const std::string& field_name,
                                                        LynxGetPropertyCallback get_callback,
                                                        LynxSetPropertyCallback set_callback) {
        Field field;
        if (get_callback != 0) {
            field.get_callback = get_callback;
        }
        if (set_callback != 0) {
            field.set_callback = set_callback;
        }
        fields_[field_name] = field;
    }

    base::ScopedPtr<LynxValue>
    LynxObjectTemplate::MethodCallback(const std::string& name, base::ScopedPtr<LynxArray>& value) {
        auto it = methods_.find(name);
        if (it != methods_.end()) {
            LynxMethodCallback callback = it->second;
            return (*callback)(this, value);
        }
        return base::ScopedPtr<LynxValue>();
    }

    void LynxObjectTemplate::SetPropertyCallback(const std::string& name,
                                                 base::ScopedPtr<LynxValue> value) {
        auto it = fields_.find(name);
        if (it != fields_.end()) {
            Field field = it->second;
            LynxSetPropertyCallback callback = field.set_callback;
            if (callback != 0) {
                (*callback)(this, value);
            }
        }
    }

    base::ScopedPtr<LynxValue> LynxObjectTemplate::GetPropertyCallback(const std::string& name) {
        auto it = fields_.find(name);
        if (it != fields_.end()) {
            Field field = it->second;
            LynxGetPropertyCallback callback = field.get_callback;
            if (callback != 0) {
                return (*callback)(this);
            }
        }
        return base::ScopedPtr<LynxValue>();
    }
}