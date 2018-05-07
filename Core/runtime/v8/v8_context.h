// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_V8_CONTEXT_H_
#define LYNX_RUNTIME_V8_V8_CONTEXT_H_

#include <string>

#include "v8.h"
#include "runtime/js/js_context.h"

namespace jscore {

class V8Context : public JSContext {
 public:
    explicit V8Context() : JSContext() {}
    virtual ~V8Context();
    virtual void Initialize(JSVM* vm,  Runtime* runtime);
    virtual std::string RunScript(const char* source);
    virtual void LoadUrl(const std::string& url);
    virtual void AddJavaScriptInterface(const std::string &name,
                                        base::ScopedPtr<LynxObjectPlatform> object);
    virtual void OnLayoutFileParseFinished();

    v8::Local<v8::Context> GetContext() {
        v8::Isolate* isolate = GetVM<v8::Isolate*>();
        return context_.Get(isolate);
    }

 private:
    static std::string GetErrorMessage(const v8::Local <v8::Message> &message,
                                       const v8::Local <v8::Value> &error);

    static std::string GetErrorStackTrace(
        const v8::Local<v8::StackTrace>& stackTrace);
    static void OnUncaughtError(v8::Handle<v8::Message> message,
                                v8::Handle<v8::Value> error);

    void Clear();
    v8::Persistent<v8::Context> context_;

    DISALLOW_COPY_AND_ASSIGN(V8Context);

};
}  // namespace jscore

#endif  // LYNX_RUNTIME_V8_V8_CONTEXT_H_
