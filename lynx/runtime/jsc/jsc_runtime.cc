// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/jsc_runtime.h"

#include "base/debug/memory_debug.h"


#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/jsc_helper.h"

namespace jscore {
    
    JSCRuntime::JSCRuntime()
        : Runtime(lynx_new JSCContext()) {
    }
    
    JSCRuntime::~JSCRuntime() {
    }
}
