// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_IOS_OC_HELPER_H_
#define LYNX_BASE_IOS_OC_HELPER_H_

#import <UIKit/UIKit.h>

#include "runtime/base/lynx_value.h"
#include "base/scoped_ptr.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_map.h"

namespace base {
    namespace ios {
        class OCHelper {
        public:
            static NSString* ConvertToOCString(const std::string& str);
            static NSString* ConvertToOCString(jscore::LynxValue* value);
            static id ConvertToOCValue(jscore::LynxValue* value);
            static NSArray* ConvertToOCArray(jscore::LynxArray* array);
            static NSDictionary* ConvertToOCMap(jscore::LynxMap* object);
            
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(NSArray *array);
            static base::ScopedPtr<jscore::LynxMap> ConvertToLynxMap(NSDictionary *dic);
            static base::ScopedPtr<jscore::LynxValue> ConvertToLynxValue(id value);
        };
    }
}

#endif // LYNX_BASE_IOS_IOS_HELPER_H_
