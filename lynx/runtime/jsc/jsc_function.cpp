// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/jsc_function.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/object_wrap.h"
#include "runtime/jsc/objects/object_template.h"
#include "runtime/jsc/jsc_helper.h"
#include <sstream>
#include "runtime/jsc/Performance.h"

namespace jscore {
    JSCFunction::JSCFunction(JSCContext* context, JSObjectRef target, JSObjectRef function) : LynxFunction(context) {
        JSContextRef ctx = static_cast<JSCContext*>(context_)->GetContext();
        std::stringstream stream;
        stream << reinterpret_cast<intptr_t>(function);
        js_function_key_ = stream.str();
        // clear stream
        stream.str("");
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

    void JSCFunction::Run(void* target, LynxArray* args) {
        Performance p("invoke_void_function");
        if (target == 0) {
            return;
        }
        JSContextRef ctx = static_cast<JSCContext*>(context_)->GetContext();
        JSObjectRef target_object;
        if ((intptr_t) target == (intptr_t) TargetState::Global) {
            target_object = JSContextGetGlobalObject(ctx);
        } else {
            target_object = static_cast<LynxObjectTemplate *>(target)->object_wrap()->js_ref();
        }

        base::ScopedPtr<JSValueRef[]> array;
        int argc = 0;
        if (args != 0) {
            array.Reset(JSCHelper::ConvertToJSValueRefArray(ctx, args));
            argc = args->Size();
        }

        for (int i = 0; i < argc; i++) {
            // register target
            if (JSValueIsObject(ctx, array[i])) {
                JSCHelper::SetValueProperty(ctx,
                                            (JSObjectRef) array[i],
                                            "target",
                                            target_object,
                                            kJSPropertyAttributeReadOnly, NULL);
            }
        }
        JSValueRef exception = nullptr;

        JSStringRef function_key = JSStringCreateWithUTF8CString(lynx_function_key_.c_str());
        JSObjectRef function = (JSObjectRef)JSObjectGetProperty(ctx, JSContextGetGlobalObject(ctx), function_key, NULL);
        JSObjectCallAsFunction(ctx, function, target_object, argc, array.Get(), &exception);
        JSStringRelease(function_key);

        if (exception) {
            int type = JSValueGetType(ctx, exception);

            std::string str = JSCHelper::ConvertToString(ctx, exception);
            if (!str.empty()) {
                LOGD("lynx-js-console", "js error: %s", str.c_str());
            }
        }
    }
}
