// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_OBJECTS_BODY_OBJECT_H_
#define LYNX_RUNTIME_V8_OBJECTS_BODY_OBJECT_H_

#if USING_V8

#include "runtime/v8/objects/element_object.h"
#include "runtime/v8/object_wrap.h"

namespace jscore {
class V8Context;
class BodyObject : public ElementObject {
 public:
  static void BindingClass(v8::Isolate* isolate,
                           v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
                           V8Context* context,
                           v8::Persistent<v8::FunctionTemplate>& constructor);
  static v8::Local<v8::Value> Create(v8::Isolate* isolate,
                                     v8::Persistent<v8::FunctionTemplate>& constructor);

 private:
  explicit BodyObject(lynx::RenderObject* render_object);
  virtual ~BodyObject();

  static void NewBodyCallback(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void GetClientWidthCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value> &info);

  static void GetClientHeightCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value> &info);
};
}  // namespace jscore
#endif
#endif  // LYNX_RUNTIME_V8_OBJECTS_BODY_OBJECT_H_
