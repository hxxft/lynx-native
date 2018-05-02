// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_FUNCTION_H_
#define LYNX_RUNTIME_JSC_JSC_FUNCTION_H_

#include "runtime/base/lynx_function.h"
#include "base/scoped_ptr.h"
#include <JavaScriptCore/JavaScript.h>

namespace jscore {
    
    class JSCContext;
    
    class JSCFunction : public LynxFunction {
    public:
        JSCFunction(JSCContext* context, JSObjectRef function);
        virtual ~JSCFunction();
        virtual void Run(void* target, LynxArray* args);
    private:
        std::string lynx_function_key_;
    };
}

#endif // LYNX_RUNTIME_JSC_JSC_FUNCTION_H_
