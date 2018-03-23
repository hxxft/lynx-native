// Copyright 2017 The Lynx Authors. All rights reserved.

#include "timeout_callback.h"

#include "v8_helper.h"

#include "runtime/v8/v8_context.h"

namespace jscore {

    void TimeoutCallback::BindingCallback(v8::Isolate* isolate, v8::Local <v8::ObjectTemplate>
                      &globalObjectTemplate, V8Context* context) {
        auto extendData = v8::External::New(isolate, context);
        globalObjectTemplate->Set(V8Helper::ConvertToV8String(isolate, "setTimeout"), v8::FunctionTemplate::New(isolate, SetTimeoutCallback, extendData));
        globalObjectTemplate->Set(V8Helper::ConvertToV8String(isolate, "clearTimeout"), v8::FunctionTemplate::New(isolate, ClearTimeoutCallback, extendData));
        globalObjectTemplate->Set(V8Helper::ConvertToV8String(isolate, "setInterval"), v8::FunctionTemplate::New(isolate, SetIntervalCallback, extendData));
        globalObjectTemplate->Set(V8Helper::ConvertToV8String(isolate, "clearInterval"), v8::FunctionTemplate::New(isolate, ClearTimeoutCallback, extendData));
    }

    void TimeoutCallback::SetTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
        if (args.Length() == 2) {
            v8::Isolate* isolate = args.GetIsolate();
            V8Context* context = reinterpret_cast<V8Context*>(args.Data().As<v8::External>()->Value());
            int time = args[1]->Int32Value();
            context->runtime()->thread_manager()->RunOnJSThreadDelay(
                    lynx_new TimeoutCallback(
                            lynx_new V8JSFunction(
                                    context,
                                    isolate->GetCurrentContext()->Global(),
                                    args[0])),
                    time);
        }
    }

    void TimeoutCallback::ClearTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value>& args){

    }

    void TimeoutCallback::SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value>& args){
        if (args.Length() == 2) {
            v8::Isolate* isolate = args.GetIsolate();
            V8Context* context = reinterpret_cast<V8Context*>(args.Data().As<v8::External>()->Value());
            int time = args[1]->Int32Value();
            context->runtime()->thread_manager()->RunOnJSThreadInterval(
                    lynx_new TimeoutCallback(
                            lynx_new V8JSFunction(
                                    context,
                                    isolate->GetCurrentContext()->Global(),
                                    args[0])),
                    time);
        }
    }

    void TimeoutCallback::Run() {
        js_function_->Run(reinterpret_cast<void*>(TargetState::Global), NULL);
    }
}
