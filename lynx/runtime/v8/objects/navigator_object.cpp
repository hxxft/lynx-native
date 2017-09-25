// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "runtime/config.h"
#include "runtime/v8/objects/navigator_object.h"
#include "runtime/v8/v8_helper.h"

using v8::Local;
using v8::Value;
using v8::PropertyCallbackInfo;

namespace jscore {

NavigatorObject::NavigatorObject() {
}

NavigatorObject::~NavigatorObject() {
}

void NavigatorObject::BindingClass(
    v8::Isolate* isolate,
    v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
    v8::Persistent<v8::FunctionTemplate>& constructor) {
    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl =
        v8::FunctionTemplate::New(isolate, NewNavigatorCallback);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Navigator"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "userAgent"),
        GetUserAgentCallback,
        NULL);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "appCodeName"),
        GetAppCodeNameCallback,
        NULL);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "appName"),
        GetAppNameCallback,
        NULL);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "platform"),
        GetPlatformCallback,
        NULL);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "appVersion"),
        GetAppVersionCallback,
        NULL);

    constructor.Reset(isolate, tpl);
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "Navigator"),
        tpl);
}

void NavigatorObject::NewNavigatorCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.IsConstructCall()) {
        NavigatorObject* obj = lynx_new NavigatorObject();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
}

v8::Local<v8::Value> NavigatorObject::Create(
    v8::Isolate* isolate,
    v8::Persistent<v8::FunctionTemplate>& constructor) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::FunctionTemplate> cons_tpl =
        v8::Local<v8::FunctionTemplate>::New(isolate, constructor);
    auto cons = cons_tpl->GetFunction(context).ToLocalChecked();
    v8::Local<v8::Object> result =
        cons->NewInstance(context, 0, nullptr).ToLocalChecked();
    return result;
}

void NavigatorObject::GetUserAgentCallback(
    Local<v8::String> property,
    const PropertyCallbackInfo<Value>& info) {
    NavigatorObject* obj = ObjectWrap::Unwrap<NavigatorObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(),
                                    obj->user_agent()));
}

void NavigatorObject::GetAppCodeNameCallback(
    Local<v8::String> property,
    const PropertyCallbackInfo<Value>& info) {
    NavigatorObject* obj = ObjectWrap::Unwrap<NavigatorObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(),
                                    obj->app_name()));
}

void NavigatorObject::GetAppVersionCallback(
    Local<v8::String> property,
    const PropertyCallbackInfo<Value>& info) {
    NavigatorObject* obj = ObjectWrap::Unwrap<NavigatorObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(),
                                    obj->app_version()));
}

void NavigatorObject::GetPlatformCallback(
    Local<v8::String> property,
    const PropertyCallbackInfo<Value>& info) {
    NavigatorObject* obj = ObjectWrap::Unwrap<NavigatorObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(),
                                    obj->platform()));
}

void NavigatorObject::GetAppNameCallback(
    Local<v8::String> property,
    const PropertyCallbackInfo<Value>& info) {
    NavigatorObject* obj = ObjectWrap::Unwrap<NavigatorObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(),
                                    obj->app_name()));
}

}
#endif
