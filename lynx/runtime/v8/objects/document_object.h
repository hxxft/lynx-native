// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_DOCUMENT_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_DOCUMENT_OBJECT_H_

#if USING_V8

#include "runtime/v8/object_wrap.h"

namespace jscore {
class V8Context;
class DocumentObject : public ObjectWrap {
 public:
  static void BindingClass(
    v8::Isolate* isolate,
    v8::Local<v8::ObjectTemplate> &globalObjectTemplate,
    V8Context* context,
    v8::Persistent<v8::FunctionTemplate>& constructor);
  static  v8::Local<v8::Value> Create(
    v8::Isolate* isolate,
    v8::Persistent<v8::FunctionTemplate>& constructor);

 private:
  DocumentObject();
  virtual ~DocumentObject();


  static void NewDocumentCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args);

  static void CreateElementCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args);

  static void CreateDomCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args);

  static void AddEventListenerCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args);

  static void RemoveEventListenerCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args);

  static void DispatchEventCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args);

  static void CreateEventCallback(
    const v8::FunctionCallbackInfo <v8::Value> &args);

  static void SetDomainCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info);

  static void GetDomainCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void GetCookieCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info);

  static void SetCookieCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info);
};
}
#endif

#endif  // LYNX_RUNTIME_V8_OBJECTS_DOCUMENT_OBJECT_H_
