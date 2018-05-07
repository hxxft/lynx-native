
// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_CONTEXT_H_
#define LYNX_RUNTIME_JSC_JSC_CONTEXT_H_

#include <JavaScriptCore/JavaScript.h>
#include "base/debug/memory_debug.h"
#include "runtime/js/js_context.h"

namespace jscore {
    class ClassTemplate;

    class JSCContext : public JSContext {
    public:
        JSCContext();

        virtual ~JSCContext();
        
        virtual void Initialize(JSVM* vm, Runtime* runtime);
        virtual std::string RunScript(const char* source);
        virtual void LoadUrl(const std::string& url);
        virtual void AddJavaScriptInterface(const std::string &name,
                                            base::ScopedPtr<LynxObjectPlatform> object);
        virtual void OnLayoutFileParseFinished();

        JSGlobalContextRef GetContext() {
            return context_;
        }
    private:
        JSGlobalContextRef context_;
        DISALLOW_COPY_AND_ASSIGN(JSCContext);
    };
}

#endif  // LYNX_RUNTIME_JSC_JSC_CONTEXT_H_
