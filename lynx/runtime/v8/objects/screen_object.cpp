// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "config/global_config_data.h"
#include "runtime/v8/objects/screen_object.h"

#include "v8.h"

#include "runtime/v8/v8_helper.h"

using v8::Isolate;
using v8::Local;
using v8::ObjectTemplate;
using v8::FunctionTemplate;
using v8::Value;
using v8::FunctionCallbackInfo;
using v8::Context;
using v8::PropertyCallbackInfo;
using v8::Number;

namespace jscore {

ScreenObject::ScreenObject() {
}

ScreenObject::~ScreenObject() {
}

void ScreenObject::BindingClass(
    Isolate* isolate,
    Local <ObjectTemplate> &globalObjectTemplate,
    v8::Persistent<FunctionTemplate>& constructor) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl =
        FunctionTemplate::New(isolate, NewScreenCallback);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Screen"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "width"),
        GetWidthCallback,
        NULL);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "height"),
        GetHeightCallback,
        NULL);

    constructor.Reset(isolate, tpl);
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "Screen"), tpl);
}

void ScreenObject::NewScreenCallback(
    const FunctionCallbackInfo<Value>& args) {

    if (args.IsConstructCall()) {
        ScreenObject *obj = lynx_new ScreenObject();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
}

Local<Value> ScreenObject::Create(
    Isolate* isolate,
    v8::Persistent<FunctionTemplate>& constructor) {
    Local<Context> context = isolate->GetCurrentContext();
    Local<FunctionTemplate> cons_tpl =
        Local<FunctionTemplate>::New(isolate, constructor);
    auto cons = cons_tpl->GetFunction(context).ToLocalChecked();
    Local<v8::Object> result =
        cons->NewInstance(context, 0, nullptr).ToLocalChecked();
    return result;
}

void ScreenObject::GetWidthCallback(
    Local<v8::String> property,
    const PropertyCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(
        Number::New(info.GetIsolate(),
                    config::GlobalConfigData::GetInstance()->screen_width()));
}

void ScreenObject::GetHeightCallback(
    Local<v8::String> property,
    const PropertyCallbackInfo<Value>& info) {
    info.GetReturnValue().Set(
        Number::New(info.GetIsolate(),
                    config::GlobalConfigData::GetInstance()->screen_height()));
}

}  // namespace jscore
#endif
