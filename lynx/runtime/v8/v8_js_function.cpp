// Copyright 2017 The Lynx Authors. All rights reserved.

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "runtime/runtime.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_js_function.h"
#include "runtime/v8/object_wrap.h"
#include "runtime/v8/v8_context.h"

using v8::Isolate;
using v8::Context;
using v8::Local;

namespace jscore {

V8JSFunction::V8JSFunction(V8Context* context,
                           v8::Local<v8::Object> target,
                           v8::Local<v8::Value> function) : LynxFunction(context) {
    js_function_key_ = reinterpret_cast<intptr_t>(
            *(reinterpret_cast<v8::internal::Object**>(*function)));
    target->ForceSet(v8::Number::New(
            static_cast<v8::Isolate*>(context_->GetVM()),
            reinterpret_cast<intptr_t>(js_function_key_)), function);
}

V8JSFunction::~V8JSFunction() {}

void V8JSFunction::Run(void* target, void* args) {

    v8::Isolate* isolate = static_cast<v8::Isolate*>(context_->GetVM());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context =
            v8::Local<v8::Context>::New(isolate,  static_cast<V8Context*>(context_)->GetContext());
    Context::Scope context_scope(context);

    if (target == (int)TargetState::Empty) return;
    Local<v8::Object> target_object;
    if ((int)target == (int)TargetState::Global) {
        target_object = context->Global();
    } else {
        ObjectWrap* wrap = reinterpret_cast<ObjectWrap*>(target);
        if (wrap == (int)TargetState::Empty) return;
        target_object = wrap->handle(isolate);
    }
    Local<v8::Function> func = Local<v8::Function>::Cast(
            target_object->Get(v8::Number::New(isolate, reinterpret_cast<intptr_t>(js_function_key_))));
    if (*func == 0) return;
    JNIEnv* env = base::android::AttachCurrentThread();
    Local<v8::Array> jsArgs;
    if ((int) args != 0) {
        jsArgs = V8Helper::ConvertToV8Array(
                isolate,
                env,
                (jobjectArray)reinterpret_cast<
                        base::android::ScopedGlobalJavaRef<jobjectArray>*>(args)->Get());
    } else {
        jsArgs = v8::Array::New(isolate);
    }
    int argc = jsArgs->Length();
    v8::Local<v8::Value> arguments[argc];
    for (int i = 0; i < argc; i++) {
        arguments[i] = jsArgs->Get(i);

        // register target
        if (arguments[i]->IsObject()) {
            arguments[i]->ToObject(context).ToLocalChecked()
                    ->Set(V8Helper::ConvertToV8String(isolate, "target"), target_object);
        }
    }
    func->Call(target_object, argc, arguments);
}
}
