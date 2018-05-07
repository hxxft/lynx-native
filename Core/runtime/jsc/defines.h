// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef RUNTIME_JSC_DEFINES
#define RUNTIME_JSC_DEFINES

#include "runtime/jsc/base.h"
#include "runtime/jsc/object_wrap.h"
#include "runtime/jsc/jsc_helper.h"

// For method callback definition
#define METHOD_CALLBACK(method_name) method_name ## Callback
#define DEFINE_METHOD_CALLBACK(clazz, c_method)                                                     \
static JSValueRef METHOD_CALLBACK(c_method)(JSContextRef ctx, JSObjectRef function,                 \
                                            JSObjectRef thiz, size_t argc,                          \
                                            const JSValueRef argv[], JSValueRef *exception) {       \
    clazz* obj = jscore::JSCObjectWrap::Unwrap<clazz>(thiz);                                        \
    auto arguments = jscore::JSCHelper::ConvertToLynxArray(ctx,                                     \
                                 const_cast<JSValueRef *>(argv), argc);                             \
    return jscore::JSCHelper::ConvertToJSValue(ctx, obj->c_method(arguments).Get());                \
}

#define DEFINE_GROUP_METHOD_CALLBACK(clazz, c_method)                                               \
static JSValueRef METHOD_CALLBACK(c_method)(JSContextRef ctx, JSObjectRef function,                 \
                                            JSObjectRef thiz, size_t argc,                          \
                                            const JSValueRef argv[], JSValueRef *exception) {       \
    clazz* obj = jscore::JSCObjectWrap::Unwrap<clazz>(thiz);                                        \
    JSStringRef name_key = JSStringCreateWithUTF8CString("name");                                   \
    JSValueRef func_name_ref = JSObjectGetProperty(ctx, function, name_key, NULL);                  \
    std::string func_name = jscore::JSCHelper::ConvertToString(ctx, func_name_ref);                 \
    JSStringRelease(name_key);                                                                      \
    auto arguments = jscore::JSCHelper::ConvertToLynxArray(ctx,                                     \
                                 const_cast<JSValueRef *>(argv), argc);                             \
    return jscore::JSCHelper::ConvertToJSValue(ctx, obj->c_method(func_name, arguments).Get());     \
}

// For field get and set callback definition
#define SET_CALLBACK(field_name)  Set ## field_name ## Callback
#define DEFINE_SET_CALLBACK(clazz, field_name)                                                      \
bool SET_CALLBACK(field_name)(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName,       \
                              JSValueRef value, JSValueRef* exception) {                            \
    clazz* obj = jscore::JSCObjectWrap::Unwrap<clazz>(object);                                      \
    base::ScopedPtr<jscore::LynxValue> js_value(jscore::JSCHelper::ConvertToLynxValue(ctx, value)); \
    obj->Set##field_name(js_value);                                                                 \
    return true;                                                                                    \
}

#define GET_CALLBACK(field_name)  Get ## field_name ## Callback
#define DEFINE_GET_CALLBACK(clazz, field_name)                                                      \
JSValueRef GET_CALLBACK(field_name)(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, \
                                    JSValueRef* exception) {                                        \
    clazz* obj = jscore::JSCObjectWrap::Unwrap<clazz>(object);                                      \
    base::ScopedPtr<jscore::LynxValue> value = obj->Get##field_name();                              \
    return jscore::JSCHelper::ConvertToJSValue(ctx, value.Get());                                   \
}


#endif //RUNTIME_JSC_DEFINES
