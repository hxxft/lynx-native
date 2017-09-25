// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_RUNTIME_H_
#define LYNX_RUNTIME_JSC_JSC_RUNTIME_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>

#include "runtime/runtime.h"

namespace jscore {
    class JSCRuntime : public Runtime {
        public:
            JSCRuntime();
            virtual ~JSCRuntime();
    };
}

#endif  // LYNX_RUNTIME_JSC_JSC_RUNTIME_H_
