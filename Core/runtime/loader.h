// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LOADER_H_
#define LYNX_RUNTIME_LOADER_H_

#include <string>
#include "net/url_request_delegate.h"
#include "runtime/base/lynx_object.h"

namespace jscore {

    class LynxFunction;

    class LoaderRequestDelegate : public net::URLRequestDelegate {
        
    public:
        LoaderRequestDelegate(JSContext* context) : LoaderRequestDelegate(context, NULL) {}
        
        LoaderRequestDelegate(JSContext* context, LynxFunction* succ)
        : LoaderRequestDelegate(context, succ, NULL) {}
        
        LoaderRequestDelegate(JSContext* context, LynxFunction* succ, LynxFunction* error)
        : context_(context), js_succ_function_(succ), js_error_function_(error) {}
        
        virtual ~LoaderRequestDelegate() {}
        
        virtual void OnSuccess(base::ScopedPtr<base::PlatformString> url,
                               base::ScopedPtr<base::PlatformString> response);
        virtual void OnFailed(base::ScopedPtr<base::PlatformString> url,
                              base::ScopedPtr<base::PlatformString> error);

    private:
        JSContext* context_;
        base::ScopedPtr<LynxFunction> js_succ_function_;
        base::ScopedPtr<LynxFunction> js_error_function_;
    };
    
    class Loader : public LynxObject {
    public:
        Loader(JSContext* context);
        ~Loader();
        void Script(const std::string& url, LynxFunction* succ_func, LynxFunction* error_func);

        base::ScopedPtr<LynxValue> Trace(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> Script(base::ScopedPtr<LynxArray>& array);

    };
}

#endif // LYNX_RUNTIME_LOADER_H_
