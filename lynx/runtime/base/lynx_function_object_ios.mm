// Copyright 2017 The Lynx Authors. All rights reserved.

#include <assert.h>
#include "runtime/base/lynx_function_object_ios.h"
#include "runtime/base/lynx_value.h"
#include "base/ios/oc_helper.h"

namespace jscore {
    
    LynxFunctionObjectIOS::LynxFunctionObjectIOS(LYXFunctionObject* ios_object)
    : ios_object_(ios_object) {
        base::ScopedPtr<LynxArray> array = LynxValue::MakeArrayScoped(base::ios::OCHelper::ConvertToLynxArray([ios_object_ methodNames]));
        for (int i = 0; i < array->Size(); ++i) {
            LynxValue* value = array->Get(i);
            assert(value->type_ == LynxValue::Type::VALUE_STRING);
            RegisterMethodCallback(value->data_.str, ExecCallback);
        }
    }
    
    LynxFunctionObjectIOS::~LynxFunctionObjectIOS() {
        ios_object_ = NULL;
    }
    
    base::ScopedPtr<LynxValue>
    LynxFunctionObjectIOS::ExecCallback(LynxFunctionObject* object,
                                            const std::string& name,
                                            base::ScopedPtr<LynxArray> array) {
        NSString* ios_name = base::ios::OCHelper::ConvertToOCString(name);
        LYXFunctionObject *ios_object = static_cast<LynxFunctionObjectIOS*>(object)->ios_object_;
        NSArray* args = base::ios::OCHelper::ConvertToOCArray(array.Get());
        id result = [ios_object execMethod: ios_name andArgs:args];
        jscore::LynxValue* js_result = base::ios::OCHelper::ConvertToLynxValue(result);
        return LynxValue::MakeValueScoped(js_result);
    }
    
}
