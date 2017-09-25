// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "v8.h"

#include "runtime/v8/objects/loader_object.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"

namespace jscore {

LoaderObject::LoaderObject(V8Context* context) : Loader(context) {

}

LoaderObject::~LoaderObject() {

}

void LoaderObject::BindingClass(
        v8::Isolate* isolate,
        v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
        V8Context* context,
        v8::Persistent<v8::FunctionTemplate>& constructor) {
    // Prepare constructor template
    auto external = v8::External::New(isolate, context);
    v8::Local<v8::FunctionTemplate> tpl =
            v8::FunctionTemplate::New(isolate, NewLoaderCallback, external);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Loader"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    tpl->PrototypeTemplate()->Set(
            V8Helper::ConvertToV8String(isolate, "trace"),
            v8::FunctionTemplate::New(isolate, TraceCallback));
    tpl->PrototypeTemplate()->Set(
            V8Helper::ConvertToV8String(isolate, "script"),
            v8::FunctionTemplate::New(isolate, ScriptCallback));

    constructor.Reset(isolate, tpl);

    globalObjectTemplate->Set(V8Helper::ConvertToV8String(isolate, "Loader"), tpl);
}

void LoaderObject::NewLoaderCallback(const v8::FunctionCallbackInfo<v8::Value>& args) {

    if (args.IsConstructCall()) {
        V8Context* context = reinterpret_cast<V8Context*>(
                args.Data().As<v8::External>()->Value());
        LoaderObject* obj = lynx_new LoaderObject(context);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
}

v8::Local<v8::Value> LoaderObject::Create(v8::Isolate* isolate, v8::Persistent<v8::FunctionTemplate>& constructor) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::FunctionTemplate> cons_tpl = v8::Local<v8::FunctionTemplate>::New(isolate, constructor);
    auto cons = cons_tpl->GetFunction(context).ToLocalChecked();
    v8::Local<v8::Object> result = cons->NewInstance(context, 0, nullptr).ToLocalChecked();
    return result;
}

void LoaderObject::TraceCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {

}

void LoaderObject::ScriptCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    LoaderObject* obj = ObjectWrap::Unwrap<LoaderObject>(args.This());
    int size = args.Length();
    string url = size > 0 ? V8Helper::ConvertToString(args[0]->ToString()) : NULL;
    V8JSFunction* succ = size > 1 ? lynx_new V8JSFunction(static_cast<V8Context*>(obj->context()), context->Global(), args[1]) : NULL;
    V8JSFunction* error = size > 2 ? lynx_new V8JSFunction(static_cast<V8Context*>(obj->context()), context->Global(), args[2]) : NULL;
    obj->Script(url, succ, error);
}

}
#endif
