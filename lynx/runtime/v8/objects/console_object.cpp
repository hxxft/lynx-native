// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8
#include "runtime/v8/objects/console_object.h"

#include "v8.h"

#include "base/print.h"
#include "runtime/v8/v8_helper.h"

namespace jscore {

ConsoleObject::ConsoleObject() {
}

ConsoleObject::~ConsoleObject() {
}

void ConsoleObject::BindingClass(
    v8::Isolate* isolate,
    v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
    v8::Persistent<v8::FunctionTemplate>& constructor) {
    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl =
        v8::FunctionTemplate::New(isolate, NewConsoleCallback);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Console"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "log"),
        v8::FunctionTemplate::New(isolate, LogCallback));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "error"),
        v8::FunctionTemplate::New(isolate, LogCallback));

    constructor.Reset(isolate, tpl);

    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "Console"), tpl);
}

void ConsoleObject::NewConsoleCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {

    if (args.IsConstructCall()) {
        ConsoleObject* obj = lynx_new ConsoleObject();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
}

v8::Local<v8::Value> ConsoleObject::Create(
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

void ConsoleObject::LogCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args) {
    if ((args.Length() > 0)) {
        v8::String::Utf8Value message(
            V8Helper::JsonStringify(args.GetIsolate(), args[0]));
#if OS_ANDROID
        LOGD("lynx-console", "%s", *message);
#else
        printf("lynx-console: %s\n", *message);
#endif
    }
}
}  // namespace jscore
#endif
