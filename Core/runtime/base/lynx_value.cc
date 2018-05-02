// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/base/lynx_value.h"

#include "lynx_object.h"
#include "lynx_object_template.h"
#include "lynx_array.h"
#include "runtime/jsc/object_wrap.h"
#include "runtime/platform_value.h"

namespace jscore {
    LynxValue::LynxValue() {

    }

    LynxValue::LynxValue(Type type) : type_(type) {

    }

    LynxValue::~LynxValue() {
        if (type_ == VALUE_LYNX_ARRAY && data_.lynx_array != 0) {
            lynx_delete(data_.lynx_array);
        } else if (type_ == VALUE_LYNX_OBJECT && data_.lynx_object != 0) {
            lynx_delete(data_.lynx_object);
        } else if (type_ == VALUE_PLATFORM && data_.platform_value != 0) {
            lynx_delete(data_.platform_value);
        } else if (type_ == VALUE_STRING && data_.str != 0) {
            lynx_deleteA(data_.str);
        }
    }

    base::ScopedPtr<LynxValue> LynxValue::MakeValueScoped(LynxValue* js_value) {
        return base::ScopedPtr<LynxValue>(js_value);
    }

    base::ScopedPtr<LynxArray> LynxValue::MakeArrayScoped(LynxArray* lynx_array) {
        return base::ScopedPtr<LynxArray>(lynx_array);
    }

    base::ScopedPtr<LynxObject> LynxValue::MakeObjectScoped(LynxObject* lynx_object) {
        return base::ScopedPtr<LynxObject>(lynx_object);
    }

}