// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8
#ifndef RUNTIME_V8_DEFINES
#define RUNTIME_V8_DEFINES

#include "runtime/v8/base.h"
#include "runtime/v8/object_wrap.h"
#include "runtime/v8/v8_helper.h"

// For method callback definition
#define METHOD_CALLBACK(method_name) method_name ## Callback
#define DEFINE_METHOD_CALLBACK(clazz, c_method)                                                     \
static void METHOD_CALLBACK(c_method)(const v8::FunctionCallbackInfo<v8::Value>& info){             \
    v8::Isolate* isolate = info.GetIsolate();                                                       \
    auto context = isolate->GetCurrentContext();                                                    \
    clazz* obj = jscore::V8ObjectWrap::Unwrap<clazz>(info.Holder());                                \
    auto arguments = jscore::V8Helper::ConvertToLynxArray(context, info);                           \
    auto result = jscore::V8Helper::ConvertToV8Value(isolate, obj->c_method(arguments).Get());      \
    info.GetReturnValue().Set(result);                                                              \
}

#define DEFINE_GROUP_METHOD_CALLBACK(clazz, c_method)                                               \
static void METHOD_CALLBACK(c_method)(const v8::FunctionCallbackInfo<v8::Value>& info){             \
    v8::Isolate* isolate = info.GetIsolate();                                                       \
    auto context = isolate->GetCurrentContext();                                                    \
    clazz* obj = jscore::V8ObjectWrap::Unwrap<clazz>(info.Holder());                                \
    std::string func_name = jscore::V8Helper::ConvertToString(info.Callee()->GetName()->ToString());\
    auto arguments = jscore::V8Helper::ConvertToLynxArray(context, info);                           \
    auto result = jscore::V8Helper::ConvertToV8Value(isolate, obj->c_method(func_name, arguments).Get());  \
    info.GetReturnValue().Set(result);                                                              \
}

// For field get and set callback definition
#define SET_CALLBACK(field_name)  Set ## field_name ## Callback
#define DEFINE_SET_CALLBACK(clazz, field_name)                                                      \
static void SET_CALLBACK(field_name)(v8::Local<v8::String> property, v8::Local<v8::Value> value,    \
                                     const v8::PropertyCallbackInfo<void>& info){                   \
    v8::Isolate* isolate = info.GetIsolate();                                                       \
    auto context = isolate->GetCurrentContext();                                                    \
    clazz* obj = jscore::V8ObjectWrap::Unwrap<clazz>(info.This());                                  \
    base::ScopedPtr<jscore::LynxValue> js_value(jscore::V8Helper::ConvertToLynxValue(context, value));  \
    obj->Set##field_name(js_value);                                                                 \
}

#define GET_CALLBACK(field_name)  Get ## field_name ## Callback
#define DEFINE_GET_CALLBACK(clazz, field_name)                                                      \
static void GET_CALLBACK(field_name)(v8::Local<v8::String> property,                                \
                                     const v8::PropertyCallbackInfo<v8::Value>& info){              \
    v8::Isolate* isolate = info.GetIsolate();                                                       \
    clazz* obj = jscore::V8ObjectWrap::Unwrap<clazz>(info.This());                                  \
    base::ScopedPtr<jscore::LynxValue> value = obj->Get##field_name();                              \
    auto result = jscore::V8Helper::ConvertToV8Value(isolate, value.Get());                         \
    info.GetReturnValue().Set(result);                                                              \
}

#endif //RUNTIME_V8_DEFINES
#endif
