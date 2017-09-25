// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_CONSOLE_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_CONSOLE_OBJECT_H_

#if USING_V8

#include "runtime/v8/object_wrap.h"

namespace jscore {
class ConsoleObject : public ObjectWrap {
 public :
    static void BindingClass(
        v8::Isolate* isolate,
        v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
        v8::Persistent<v8::FunctionTemplate>& constructor);

    static v8::Local<v8::Value> Create(
        v8::Isolate* isolate,
        v8::Persistent<v8::FunctionTemplate>& constructor);

 private :
    ConsoleObject();
    virtual ~ConsoleObject();

    static void NewConsoleCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);
    static void LogCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);
};
}  // namespace jscore
#endif
#endif  // LYNX_RUNTIME_V8_OBJECTS_CONSOLE_OBJECT_H_
