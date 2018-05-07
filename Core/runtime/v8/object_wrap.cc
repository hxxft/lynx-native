// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/v8/object_wrap.h"

#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"
#include "runtime/base/lynx_object.h"

namespace jscore {

    V8ObjectWrap::V8ObjectWrap(V8Context* context, LynxObject* object, v8::Local<v8::Object> handle)
            : ObjectWrap(context, object), refs_(0) {
        Wrap(context->GetContext()->GetIsolate(), handle);
        OnJSObjectInitialize();
    }

    V8ObjectWrap::~V8ObjectWrap() {
        if (persistent().IsEmpty())
            return;
        if (!persistent().IsEmpty()) {
            persistent().ClearWeak();
            persistent().Reset();
        }
    }

    void V8ObjectWrap::Update() {
        lynx_delete(this);
    }

    void V8ObjectWrap::RegisterMethodCallback(const std::string &method_name,
                                              JSMethodCallback callback) {
        v8::Isolate *isolate = context_->GetVM<v8::Isolate *>();
        auto context = static_cast<V8Context*>(context_)->GetContext();
        auto function = v8::FunctionTemplate::New(isolate, callback)->GetFunction(context).ToLocalChecked();
        auto name = V8Helper::ConvertToV8String(isolate, method_name);
        function->SetName(name);
        handle(isolate)->Set(context, name, function);
    }

    void V8ObjectWrap::OnJSObjectInitialize() {
        lynx_object_->SetObjectWrap(this);
    }

    void V8ObjectWrap::OnJSObjectFinalize() {
        lynx_delete(this);
    }
}