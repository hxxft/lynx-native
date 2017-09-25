// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_WINDOW_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_WINDOW_OBJECT_H_

#if USING_V8

#include "runtime/v8/object_wrap.h"

namespace jscore {
    class V8Context;
class WindowObject : public ObjectWrap {
 public:
    static void BindingClass(
        v8::Isolate* isolate,
        v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
        V8Context* context);

 private:
    WindowObject();
    virtual ~WindowObject();

    static void PromptCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);
    static void AddEventListenerCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
    static void RemoveEventListenerCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
    static void DispatchEventCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
    static void OpenCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
    static void FetchCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
    static void GetDevicePixelRatioCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
};
}  // namespace jscore
#endif
#endif  // LYNX_RUNTIME_V8_OBJECTS_WINDOW_OBJECT_H_
