// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/loader.h"
#include "net/url_request.h"
#include "net/url_request_context.h"
#include "runtime/runtime.h"
#include "runtime/global.h"
#include "runtime/base/lynx_function.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/js/class_template.h"
#include "runtime/js/js_context.h"

#include "base/debug/memory_debug.h"

namespace jscore {

    #define FOR_EACH_METHOD_BINDING(V)   \
        V(Loader, Trace)                 \
        V(Loader, Script)

    // Defines methods and fields
    FOR_EACH_METHOD_BINDING(DEFINE_METHOD_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Loader)
        FOR_EACH_METHOD_BINDING(REGISTER_METHOD_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    Loader::Loader(JSContext* context) : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)) {
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

    base::ScopedPtr<LynxValue> Loader::Trace(base::ScopedPtr<LynxArray>& array) {
        return base::MakeScopedPtr<LynxArray>(NULL);
    }

    base::ScopedPtr<LynxValue> Loader::Script(base::ScopedPtr<LynxArray>& array) {
        if (array.Get() != NULL && array->Size() > 0
                && array->Get(0)->type_ == LynxValue::Type::VALUE_STRING) {
            std::string url = array->Get(0)->data_.str;
            LynxFunction* succ = array->Size() > 1
                               && array->Get(1)->type_ == LynxValue::Type::VALUE_LYNX_FUNCTION ?
                               array->Get(1)->data_.lynx_function : NULL;
            LynxFunction* error = array->Size() > 2
                                && array->Get(2)->type_ == LynxValue::Type::VALUE_LYNX_FUNCTION ?
                                array->Get(2)->data_.lynx_function : NULL;
            Script(url, succ, error);
        }
        return base::MakeScopedPtr<LynxValue>(NULL);
    }
    
    void LoaderRequestDelegate::OnFailed(base::ScopedPtr<base::PlatformString> url,
                                         base::ScopedPtr<base::PlatformString> error) {
        if (js_error_function_.Get() != NULL) {
            js_error_function_->Run(context_->global(), NULL);
        }
    }
    
    void LoaderRequestDelegate::OnSuccess(base::ScopedPtr<base::PlatformString> url,
                                          base::ScopedPtr<base::PlatformString> response) {
        if (js_succ_function_.Get() != NULL) {
            context_->RunScript(response->GetUTFChars());
            js_succ_function_->Run(context_->global(), NULL);
        }
    }
}
