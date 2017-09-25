// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_V8_CONTEXT_H_
#define LYNX_RUNTIME_V8_V8_CONTEXT_H_

#include <string>

#include "v8.h"
#include "runtime/js_context.h"


namespace jscore {

class DocumentObject;
class NavigatorObject;
class ConsoleObject;
class ScreenObject;
class LocationObject;
class HistoryObject;
class LoaderObject;
class V8Context : public JSContext {
 public:
    explicit V8Context() : JSContext() {}
    virtual ~V8Context();
    virtual void Initialize(JSVM* vm,  Runtime* runtime);
    virtual void RunScript(const char* source);
    virtual void LoadUrl(const std::string& url);

    v8::Persistent<v8::Context>& GetContext() {
        return context_;
    }

    HistoryObject* history_object() {
        return history_object_;
    }

    v8::Persistent<v8::FunctionTemplate>& element_constructor() {
        return element_constructor_;
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

    v8::Persistent<v8::FunctionTemplate> console_constructor_;
    v8::Persistent<v8::FunctionTemplate> document_constructor_;
    v8::Persistent<v8::FunctionTemplate> body_constructor_;
    v8::Persistent<v8::FunctionTemplate> element_constructor_;
    v8::Persistent<v8::FunctionTemplate> navigator_constructor_;
    v8::Persistent<v8::FunctionTemplate> screen_constructor_;
    v8::Persistent<v8::FunctionTemplate> location_constructor_;
    v8::Persistent<v8::FunctionTemplate> history_constructor_;
    v8::Persistent<v8::FunctionTemplate> loader_constructor_;

    // WindowObject* window_object_;
    DocumentObject* document_object_;
    NavigatorObject* navigator_object_;
    ConsoleObject* console_object_;
    ScreenObject* screen_object_;
    LocationObject* location_object_;
    HistoryObject* history_object_;
    LoaderObject* loader_object_;
};
}  // namespace jscore

#endif  // LYNX_RUNTIME_V8_V8_CONTEXT_H_
