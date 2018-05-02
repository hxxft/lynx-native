// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_HISTORY_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_HISTORY_OBJECT_H_

#include <vector>
#include <string>

#include "runtime/v8/object_wrap.h"
#include "runtime/history.h"

namespace jscore {

class V8Context;

class HistoryObject : public ObjectWrap, public History{
 public:
    static void BindingClass(
        v8::Isolate* isolate,
        v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
        V8Context* context,
        v8::Persistent<v8::FunctionTemplate>& constructor);
    static v8::Local<v8::Value> Create(
        v8::Isolate* isolate,
        v8::Persistent<v8::FunctionTemplate>& constructor);

//    void ReloadUrl(const std::string& url);
//    void GoToUrl(const std::string& url);
//    void GoToOffset(int offset);

 private:
    explicit HistoryObject(V8Context* context);
    virtual ~HistoryObject();

    static void NewHistoryCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);
    static void BackCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);
    static void ForwardCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
    static void GoCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
    static void GetLengthCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
};

}  // namespace jscore
#endif  // LYNX_RUNTIME_V8_OBJECTS_HISTORY_OBJECT_H_
