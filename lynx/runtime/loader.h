// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LOADER_H_
#define LYNX_RUNTIME_LOADER_H_

#include <string>
#include "base/weak_ptr.h"
#include "net/url_request_delegate.h"
#include "runtime/base/lynx_object_template.h"

namespace jscore {

    class JSContext;
    class LynxFunction;

    class LoaderRequestDelegate : public net::URLRequestDelegate {
        
    public:
        LoaderRequestDelegate(JSContext* context) : LoaderRequestDelegate(context, NULL) {}
        
        LoaderRequestDelegate(JSContext* context, LynxFunction* succ)
        : LoaderRequestDelegate(context, succ, NULL) {}
        
        LoaderRequestDelegate(JSContext* context, LynxFunction* succ, LynxFunction* error)
        : context_(context), js_succ_function_(succ), js_error_function_(error) {}
        
        virtual ~LoaderRequestDelegate() {}
        
        virtual void OnSuccess(const base::PlatformString& url,
                               const base::PlatformString& response);
        virtual void OnFailed(const base::PlatformString& url,
                              const base::PlatformString& error);

    private:
        JSContext* context_;
        base::ScopedPtr<LynxFunction> js_succ_function_;
        base::ScopedPtr<LynxFunction> js_error_function_;
    };
    
    class Loader : public LynxObjectTemplate {
    public:
        Loader(JSContext* context);
        ~Loader();
        void Script(const std::string& url, LynxFunction* succ_func, LynxFunction* error_func);

        inline JSContext* context() {
            return context_;
        }
    private:

        static base::ScopedPtr<LynxValue>
        TraceCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        static base::ScopedPtr<LynxValue>
        ScriptCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);

        JSContext* context_;
    };
}

#endif // LYNX_RUNTIME_LOADER_H_
