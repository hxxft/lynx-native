// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/v8/v8_function.h"

#include <sstream>
#include "runtime/runtime.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/object_wrap.h"
#include "runtime/v8/v8_context.h"

namespace jscore {

V8Function::V8Function(V8Context* context, v8::Local<v8::Value> function) : LynxFunction(context) {
    auto isolate = context_->GetVM<v8::Isolate*>();
    auto global = context->GetContext()->Global();
    std::stringstream stream;
    stream << reinterpret_cast<intptr_t>(*(reinterpret_cast<v8::internal::Object**>(*function)));
    js_function_key_ = stream.str();
    // clear stream
    stream.str("");
    stream << "func";
    stream << reinterpret_cast<intptr_t>(this);
    lynx_function_key_ = stream.str();
    global->ForceSet(V8Helper::ConvertToV8String(isolate, lynx_function_key_), function);
}

V8Function::~V8Function() {

    v8::Isolate* isolate = context_->GetVM<v8::Isolate*>();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = static_cast<V8Context*>(context_)->GetContext();
    if (!context.IsEmpty()) {
        v8::Context::Scope context_scope(context);
        context->Global()->Delete(V8Helper::ConvertToV8String(isolate, lynx_function_key_));
    }
}

void V8Function::Run(LynxObject* target, LynxArray* args) {

    v8::Isolate* isolate = context_->GetVM<v8::Isolate*>();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = static_cast<V8Context*>(context_)->GetContext();
    v8::Context::Scope context_scope(context);

    if (target == NULL) return;
    v8::Local<v8::Object> target_object;
    V8ObjectWrap* wrap = static_cast<V8ObjectWrap*>(target->object_wrap());
    target_object = wrap->handle(isolate);
    v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(
            context->Global()->Get(V8Helper::ConvertToV8String(isolate, lynx_function_key_)));
    int argc = 0;
    v8::Local<v8::Array> js_args;
    if (args != NULL) {
        js_args = V8Helper::ConvertToV8Array(isolate, args);
        argc = js_args->Length();
    }
    v8::Local<v8::Value> *arguments = lynx_new v8::Local<v8::Value>[argc];
    for (int i = 0; i < argc; i++) {
        arguments[i] = js_args->Get(i);
    }
    func->Call(context, target_object, argc, arguments);
    lynx_deleteA(arguments);
}
}
