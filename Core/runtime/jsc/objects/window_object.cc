// Copyright 2017 The Lynx Authors. All rights reserved.

#include "window_object.h"

#include "config/global_config_data.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/jsc/class_wrap.h"

namespace jscore {
    
    static JSValueRef OpenCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                           size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef PromptCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                            size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef AddEventListenerCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                            size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        return JSValueMakeNull(ctx);
    }
    
    static JSValueRef RemoveEventListenerCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                            size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef DispatchEventCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz,
                                            size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        return JSValueMakeUndefined(ctx);
    }
    
    static JSValueRef GetDevicePixelRatioCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        int density = (int) config::GlobalConfigData::GetInstance()->screen_density();
        return JSValueMakeNumber(ctx, density);
    }
    
    void WindowObject::BindingClass(ClassWrap* class_wrap) {
        class_wrap->SetJSStaticFunction("open", OpenCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
        class_wrap->SetJSStaticFunction("prompt", PromptCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
        class_wrap->SetJSStaticFunction("addEventListener", AddEventListenerCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
        class_wrap->SetJSStaticFunction("removeEventListener", RemoveEventListenerCallback,kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
        class_wrap->SetJSStaticFunction("dispatchEvent", DispatchEventCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
        class_wrap->SetJSStaticValue("devicePixelRatio", GetDevicePixelRatioCallback, NULL, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
    }
}
