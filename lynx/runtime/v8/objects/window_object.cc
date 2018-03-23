// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "runtime/v8/objects/window_object.h"

#include "v8.h"

#include "config/global_config_data.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"

using v8::Isolate;
using v8::Local;
using v8::FunctionTemplate;
using v8::ObjectTemplate;
using v8::External;
using v8::Value;
using v8::FunctionCallbackInfo;


namespace jscore {

WindowObject::WindowObject() {
}

WindowObject::~WindowObject() {
}

void WindowObject::BindingClass(
    Isolate* isolate,
    Local <ObjectTemplate> &globalObjectTemplate,
    V8Context* context) {
    auto external = External::New(isolate, context);
    // Prototype
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "open"),
        FunctionTemplate::New(isolate, OpenCallback));
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "prompt"),
        FunctionTemplate::New(isolate, PromptCallback));
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "addEventListener"),
        FunctionTemplate::New(isolate, AddEventListenerCallback));
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "removeEventListener"),
        FunctionTemplate::New(isolate, RemoveEventListenerCallback));
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "dispatchEvent"),
        FunctionTemplate::New(isolate, DispatchEventCallback));
    globalObjectTemplate->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "devicePixelRatio"),
        GetDevicePixelRatioCallback,
        NULL);
}

void WindowObject::OpenCallback(
    const FunctionCallbackInfo<Value>& args) {
}

void WindowObject::PromptCallback(
    const FunctionCallbackInfo <Value> &args) {
}

void WindowObject::AddEventListenerCallback(
    const FunctionCallbackInfo <Value> &args) {
}

void WindowObject::RemoveEventListenerCallback(
    const FunctionCallbackInfo <Value> &args) {
}

void WindowObject::DispatchEventCallback(
    const FunctionCallbackInfo <Value> &args) {
}

void WindowObject::GetDevicePixelRatioCallback(
    Local<v8::String> property,
    const v8::PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(
        v8::Number::New(
            isolate,
            config::GlobalConfigData::GetInstance()->screen_density()));
}
}  // namespace jscore
#endif
