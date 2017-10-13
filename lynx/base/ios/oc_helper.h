// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_IOS_OC_HELPER_H_
#define LYNX_BASE_IOS_OC_HELPER_H_

#import <UIKit/UIKit.h>

#include "runtime/base/lynx_value.h"
#include "base/scoped_ptr.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_object.h"

namespace base {
    namespace ios {
        class OCHelper {
        public:
            static id ConvertToOCValue(jscore::LynxValue* value);
            static NSMutableArray* ConvertToOCArray(jscore::LynxArray* array);
            
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(NSMutableArray *array);
            static base::ScopedPtr<jscore::LynxObject> ConvertToLynxObject(NSMutableDictionary *dic);
            static base::ScopedPtr<jscore::LynxValue> ConvertToLynxValue(id value);
        };
    }
}

#endif // LYNX_BASE_IOS_IOS_HELPER_H_
