// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_LOCATION_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_LOCATION_OBJECT_H_

#if USING_V8

#include <string>

#include "runtime/v8/object_wrap.h"
#include "net/uri.h"
#include "runtime/location.h"

namespace jscore {

class V8Context;

class LocationObject : public ObjectWrap, public Location {
 public:
    static void BindingClass(
        v8::Isolate* isolate,
        v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
        V8Context* context,
        v8::Persistent<v8::FunctionTemplate>& constructor);

    static v8::Local<v8::Value> Create(
        v8::Isolate* isolate,
        v8::Persistent<v8::FunctionTemplate>& constructor);

 private:
    explicit LocationObject(V8Context* context);
    virtual ~LocationObject();

    static void NewLocationCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);

    // Replace current document with new one
    // this method does not generate a new record in the history object.
    // When using this method, the new URL will overwrite the current record in the history object.
    static void ReplaceCallback(
        const v8::FunctionCallbackInfo<v8::Value>& args);

    // While force is false
    // check if the content has changed according to If-Modified-Since of HTTP header
    // if changed, reload content, if not, nothing to do.
    // While force is true, reload content.
    static void ReloadCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);

    // This method uses to load a new document.
    static void AssignCallback(
        const v8::FunctionCallbackInfo <v8::Value> &args);

    static void GetHostNameCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetHostNameCallback(
        v8::Local<v8::String> property, v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetHashCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetHashCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetHostCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetHostCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetHrefCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetHrefCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetPortCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetPortCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetPathNameCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetPathNameCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetSearchCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetSearchCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetProtocolCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetProtocolCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

    static void GetOriginCallback(
        v8::Local<v8::String> property,
        const v8::PropertyCallbackInfo<v8::Value>& info);

    static void SetOriginCallback(
        v8::Local<v8::String> property,
        v8::Local<v8::Value> value,
        const v8::PropertyCallbackInfo<void>& info);

};
}  // namespace jscore
#endif
#endif  // LYNX_RUNTIME_V8_OBJECTS_LOCATION_OBJECT_H_
