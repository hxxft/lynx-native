// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_SCREEN_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_SCREEN_OBJECT_H_

#include "runtime/v8/object_wrap.h"

namespace jscore {
class ScreenObject : public ObjectWrap {
 public :
    static void BindingClass(v8::Isolate* isolate,
        v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
        v8::Persistent<v8::FunctionTemplate>& constructor);
    static v8::Local<v8::Value> Create(v8::Isolate* isolate,
        v8::Persistent<v8::FunctionTemplate>& constructor);

 private :
    ScreenObject();
    virtual ~ScreenObject();

    static void NewScreenCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetWidthCallback(v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetHeightCallback(v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
};
}  // namespace jscore
#endif  // LYNX_RUNTIME_V8_OBJECTS_SCREEN_OBJECT_H_
