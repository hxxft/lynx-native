
// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_CONTEXT_H_
#define LYNX_RUNTIME_JSC_JSC_CONTEXT_H_

#include "base/debug/memory_debug.h"

#include "runtime/js_context.h"
#include <JavaScriptCore/JavaScript.h>

namespace jscore {
    class ClassWrap;
    class JSCClassWrapStorage;
    
    class JSCContext : public JSContext {
    public:
        JSCContext();

        virtual ~JSCContext();
        
        virtual void Initialize(JSVM* vm, Runtime* runtime);
        virtual std::string RunScript(const char* source);
        virtual void LoadUrl(const std::string& url);
        virtual void AddJavaScriptInterface(const std::string &name,
                                            LynxFunctionObject *object);

        JSCClassWrapStorage* context_storage() {
            return class_wrap_storage_.Get();
        }
        
        JSGlobalContextRef GetContext() {
            return context_;
        }
    private:
        JSGlobalContextRef context_;
        base::ScopedPtr<JSCClassWrapStorage> class_wrap_storage_;
    };
}

#endif  // LYNX_RUNTIME_JSC_JSC_CONTEXT_H_
