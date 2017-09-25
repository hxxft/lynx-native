// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/timeout_callback.h"
#include "runtime/jsc/jsc_function.h"
#include "runtime/timed_task.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/jsc_class_wrap_storage.h"

namespace jscore {
    
    static JSValueRef SetTimeoutCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                   size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        if (JSValueIsObject(ctx, argv[0])
                && JSValueIsNumber(ctx, argv[1])
                && JSObjectIsFunction(ctx, (JSObjectRef) argv[0])) {
            JSCContext* js_context = static_cast<JSCContext*>(JSObjectGetPrivate(thiz));
            base::ScopedPtr<TimedTaskInvoker> invoker(lynx_new TimedTaskInvoker());
            JSCFunction* jsc_function = lynx_new JSCFunction(js_context, JSContextGetGlobalObject(ctx), (JSObjectRef) argv[0]);
            invoker->SetTimeout(js_context, jsc_function, (int) JSValueToNumber(ctx, argv[1], NULL));
        }
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef SetIntervalCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                   size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        if (JSValueIsObject(ctx, argv[0])
            && JSValueIsNumber(ctx, argv[1])
            && JSObjectIsFunction(ctx, (JSObjectRef) argv[0])) {
            JSCContext* js_context = static_cast<JSCContext*>(JSObjectGetPrivate(thiz));
            base::ScopedPtr<TimedTaskInvoker> invoker(lynx_new TimedTaskInvoker());
            JSCFunction* jsc_function = lynx_new JSCFunction(js_context, JSContextGetGlobalObject(ctx), (JSObjectRef) argv[0]);
            invoker->SetInterval(js_context, jsc_function, (int) JSValueToNumber(ctx, argv[1], NULL));
        }
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef ClearTimeoutCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                   size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        return JSValueMakeUndefined(ctx);
    }
    
    void TimeoutCallback::BindingClass(ClassWrap* class_wrap) {
        class_wrap->SetJSStaticFunction("setTimeout", SetTimeoutCallback, kJSPropertyAttributeNone);
        class_wrap->SetJSStaticFunction("setInterval", SetIntervalCallback, kJSPropertyAttributeNone);
        class_wrap->SetJSStaticFunction("clearTimeout", ClearTimeoutCallback, kJSPropertyAttributeNone);
    }
}