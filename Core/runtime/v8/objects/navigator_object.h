// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_NAVIGATOR_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_NAVIGATOR_OBJECT_H_

#include "runtime/v8/object_wrap.h"
#include "runtime/navigator.h"

namespace jscore {
class NavigatorObject : public ObjectWrap, public Navigator {
 public :
    static void BindingClass(
        v8::Isolate* isolate,
        v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
        v8::Persistent<v8::FunctionTemplate>& constructor);
    static v8::Local<v8::Value> Create(
        v8::Isolate* isolate,
        v8::Persistent<v8::FunctionTemplate>& constructor);

 private :
    NavigatorObject();
    virtual ~NavigatorObject();

    static void NewNavigatorCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetUserAgentCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetAppCodeNameCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetAppVersionCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetPlatformCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetAppNameCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);
};
}  // namespace jscore
#endif  // LYNX_RUNTIME_V8_OBJECTS_NAVIGATOR_OBJECT_H_
