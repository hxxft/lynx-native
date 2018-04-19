// Copyright 2017 The Lynx Authors. All rights reserved.

#include <runtime/base/lynx_value.h>
#include "runtime/runtime.h"
#include "runtime/loader.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "net/url_request.h"
#include "runtime/js_context.h"
#include "runtime/base/lynx_function.h"
#include "net/url_request_context.h"

#include "base/debug/memory_debug.h"

namespace jscore {
    Loader::Loader(JSContext* context) : context_(context) {
        set_class_name("Loader");
        RegisterMethodCallback("trace", &TraceCallback);
        RegisterMethodCallback("script", &ScriptCallback);
    }
    
    Loader::~Loader() {
        
    }

    void Loader::Script(const std::string& url, LynxFunction* succ_func, LynxFunction* error_func) {
        LoaderRequestDelegate* delegate = lynx_new LoaderRequestDelegate(context_, succ_func, error_func);
        context_->runtime()
                ->url_requset_context()
                ->CreateRequest(url, delegate)
                ->Fetch();
    }

    base::ScopedPtr<LynxValue>
    Loader::TraceCallback(LynxObjectTemplate* object,
                             base::ScopedPtr<LynxArray>& array) {
        return LynxValue::MakeValueScoped(NULL);
    }

    base::ScopedPtr<LynxValue>
    Loader::ScriptCallback(LynxObjectTemplate* object,
                             base::ScopedPtr<LynxArray>& array) {
        Loader* loader = static_cast<Loader*>(object);
        if (array.Get() != NULL && array->Size() > 0
                && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string url = array->Get(0)->data_.str;
            LynxFunction* succ = array->Size() > 1
                               && array->Get(1)->type_ == LynxValue::Type::VALUE_LYNX_FUNCTION ?
                               array->Get(1)->data_.lynx_function : NULL;
            LynxFunction* error = array->Size() > 2
                                && array->Get(2)->type_ == LynxValue::Type::VALUE_LYNX_FUNCTION ?
                                array->Get(2)->data_.lynx_function : NULL;
            loader->Script(url, succ, error);
        }
        return LynxValue::MakeValueScoped(NULL);
    }
    
    void LoaderRequestDelegate::OnFailed(base::ScopedPtr<base::PlatformString> url,
                                         base::ScopedPtr<base::PlatformString> error) {
        if (js_error_function_.Get() != NULL) {
            js_error_function_->Run(reinterpret_cast<void *>(TargetState::Global), NULL);
        }
    }
    
    void LoaderRequestDelegate::OnSuccess(base::ScopedPtr<base::PlatformString> url,
                                          base::ScopedPtr<base::PlatformString> response) {
        if (js_succ_function_.Get() != NULL) {
            context_->RunScript(response->GetUTFChars());
            js_succ_function_->Run(reinterpret_cast<void *>(TargetState::Global), NULL);
        }
    }
}
