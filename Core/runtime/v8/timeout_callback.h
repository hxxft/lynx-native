// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef JSCORE_TIMEOUT_CALLBACK_H_
#define JSCORE_TIMEOUT_CALLBACK_H_

#include "v8.h"
#include "base/task/callback.h"
#include "base/scoped_ptr.h"
#include "runtime/runtime.h"
#include "runtime/v8/v8_js_function.h"


namespace jscore {
class V8Context;
    class TimeoutCallback : public base::Clouse{
    public:

        static void BindingCallback(v8::Isolate* isolate, v8::Local <v8::ObjectTemplate>
                                    &globalObjectTemplate, V8Context* context);
        TimeoutCallback(V8JSFunction* js_function) : js_function_(js_function) {}
        ~TimeoutCallback() {}
        virtual void Run();
    private:

        static void SetTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void ClearTimeoutCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        static void SetIntervalCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

        base::ScopedPtr<V8JSFunction> js_function_;
    };
}



#endif //JSCORE_TIMEOUT_CALLBACK_H_
