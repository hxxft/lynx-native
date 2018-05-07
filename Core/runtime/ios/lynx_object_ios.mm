// Copyright 2017 The Lynx Authors. All rights reserved.

#include <assert.h>
#include "runtime/ios/lynx_object_ios.h"
#include "runtime/base/lynx_value.h"
#include "base/ios/oc_helper.h"

namespace jscore {
    
    DEFINE_GROUP_METHOD_CALLBACK(LynxObjectIOS, Exec)
    
    LynxObjectIOS::LynxObjectIOS(LxObject *ios_object)
    : ios_object_(ios_object) {
        base::ScopedPtr<LynxArray> array = base::ios::OCHelper::ConvertToLynxArray([ios_object_ methodNames]);
        for (int i = 0; i < array->Size(); ++i) {
            LynxValue* value = array->Get(i);
            assert(value->type_ == LynxValue::Type::VALUE_STRING);
            RegisterMethodCallback(value->data_.str, ExecCallback);
        }
    }
    
    LynxObjectIOS::~LynxObjectIOS() {
        ios_object_ = NULL;
    }
    
    base::ScopedPtr<LynxValue>
    LynxObjectIOS::Exec(const std::string& name, base::ScopedPtr<LynxArray> array) {
        NSString* ios_name = base::ios::OCHelper::ConvertToOCString(name);
        NSArray* args = base::ios::OCHelper::ConvertToOCArray(array.Get());
        id result = [ios_object_ execMethod: ios_name andArgs:args];
        return base::ios::OCHelper::ConvertToLynxValue(result);
    }
    
}
