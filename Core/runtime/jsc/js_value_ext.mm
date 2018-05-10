// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/jsc/js_value_ext.h"

#include "runtime/jsc/jsc_helper.h"

#include "base/ios/common.h"

bool JSValueIsArrayExt(JSContextRef ctx, JSValueRef value) {
    return (!iOS9Later && JSValueIsObject(ctx, value)
            && !JSValueIsUndefined(ctx,
                                   jscore::JSCHelper::GetProperty(ctx, (JSObjectRef) value, "length", 0)))
    || (iOS9Later && JSValueIsArray(ctx, value));
}

