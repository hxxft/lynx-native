// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/jsc_function.h"

#include <sstream>
#include "base/log/logging.h"
#include "runtime/base/lynx_array.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/object_wrap.h"
#include "runtime/jsc/jsc_helper.h"

namespace jscore {
    JSCFunction::JSCFunction(JSCContext* context, JSObjectRef function) : LynxFunction(context) {
        JSContextRef ctx = static_cast<JSCContext*>(context_)->GetContext();
        JSObjectRef target = JSContextGetGlobalObject(ctx);
        std::stringstream stream;
        stream << reinterpret_cast<intptr_t>(function);
        js_function_key_ = stream.str();
        // clear stream
        stream.str("");
        stream << "func";
        stream << reinterpret_cast<intptr_t>(this);
        lynx_function_key_ = stream.str();
        JSCHelper::SetValueProperty(ctx,
                                    target,
                                    lynx_function_key_,
                                    function,
                                    kJSPropertyAttributeNone,
                                    NULL);
    }
    
    JSCFunction::~JSCFunction() {
        JSContextRef ctx = static_cast<JSCContext*>(context_)->GetContext();
        if (ctx != NULL) {
            JSObjectRef object = JSContextGetGlobalObject(ctx);
            JSCHelper::DeleteProperty(ctx, object, lynx_function_key_, 0);
        }
    }

    void JSCFunction::Run(LynxObject* target, LynxArray* args) {
  
        JSContextRef ctx = static_cast<JSCContext*>(context_)->GetContext();
        JSObjectRef target_object = JSContextGetGlobalObject(ctx);
        if(target != nullptr) {
            auto object_wrap = static_cast<JSCObjectWrap*>(target->object_wrap());
            target_object = object_wrap->js_ref();
        }

        base::ScopedPtr<JSValueRef[]> array;
        size_t argc = 0;
        if (args != 0) {
            array.Reset(JSCHelper::ConvertToJSValueRefArray(ctx, args));
            argc = args->Size();
        }

        JSValueRef exception = nullptr;

        JSStringRef function_key = JSStringCreateWithUTF8CString(lynx_function_key_.c_str());
        JSObjectRef function = (JSObjectRef)JSObjectGetProperty(ctx, JSContextGetGlobalObject(ctx), function_key, NULL);
        JSObjectCallAsFunction(ctx, function, target_object, argc, array.Get(), &exception);
        JSStringRelease(function_key);

        if (exception) {

            std::string str = JSCHelper::ConvertToString(ctx, exception);
            if (!str.empty()) {
                context_->OnExceptionOccurred(str);
                DLOG(ERROR) << str;
            }
        }
    }
}
