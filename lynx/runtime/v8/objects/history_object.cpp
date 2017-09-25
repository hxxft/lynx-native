// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "runtime/v8/objects/history_object.h"

#include "v8.h"

#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"

namespace jscore {

HistoryObject::HistoryObject(V8Context* context) : History(context) {
}

HistoryObject::~HistoryObject() {
}

void HistoryObject::BindingClass(
    v8::Isolate* isolate,
    v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
    V8Context* context,
    v8::Persistent<v8::FunctionTemplate>& constructor) {
    // Prepare constructor template
    auto extendData = v8::External::New(isolate, context);
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(
            isolate, NewHistoryCallback, extendData);
    tpl->SetClassName(
        V8Helper::ConvertToV8String(isolate, "History"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "forward"),
        v8::FunctionTemplate::New(isolate, ForwardCallback));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "back"),
        v8::FunctionTemplate::New(isolate, BackCallback));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "go"),
        v8::FunctionTemplate::New(isolate, GoCallback));
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "length"),
        GetLengthCallback,
        NULL);

    constructor.Reset(isolate, tpl);
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "History"),
        tpl);;
}

void HistoryObject::NewHistoryCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {

    if (args.IsConstructCall()) {
        V8Context* runtime = reinterpret_cast<V8Context*>(
                                 args.Data().As<v8::External>()->Value());
        HistoryObject* obj = lynx_new HistoryObject(runtime);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
}

v8::Local<v8::Value> HistoryObject::Create(
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

void HistoryObject::BackCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    HistoryObject* obj = ObjectWrap::Unwrap<HistoryObject>(args.Holder());
    obj->Back();
    obj->Load();
}

void HistoryObject::GoCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args) {
    v8::Isolate* isolate = args.GetIsolate();
    HistoryObject* obj = ObjectWrap::Unwrap<HistoryObject>(args.Holder());
    if (args[0]->IsNumber()) {
        int offset = args[0]->NumberValue();
        obj->Go(offset);
    } else if (args[0]->IsString() || args[0]->IsStringObject()) {
        obj->Go(V8Helper::ConvertToString(args[0]->ToString()));
    }
    obj->Load();
}

void HistoryObject::ForwardCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args) {
    v8::Isolate* isolate = args.GetIsolate();
    HistoryObject* obj = ObjectWrap::Unwrap<HistoryObject>(args.Holder());
    obj->Forward();
    obj->Load();
}

void HistoryObject::GetLengthCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();
    HistoryObject* obj = ObjectWrap::Unwrap<HistoryObject>(info.This());
    info.GetReturnValue().Set(
            v8::Number::New(isolate,
                            static_cast<int>(obj->GetLength())));
}

}  // namespace jscore
#endif
