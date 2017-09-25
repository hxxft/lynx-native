// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#ifndef ELEMENT_OBJECT_H_
#define ELEMENT_OBJECT_H_

#include<unordered_map>

#include "base/scoped_ptr.h"
#include "render/render_object.h"
#include "runtime/js_function.h"
#include "runtime/v8/object_wrap.h"

namespace jscore {
class V8Context;
class ElementObject : public ObjectWrap {
public:
    static void SetFuncTemplate(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> &func_tpl, v8::Local<v8::External> extData);
    static void BindingClass(v8::Isolate* isolate,
                             v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
                             V8Context* context,
                             v8::Persistent<v8::FunctionTemplate>& constructor);
    static v8::Local<v8::Value> CreateElement(v8::Isolate* isolate, lynx::RenderObject* render_object, v8::Persistent<v8::FunctionTemplate>& constructor);
    explicit ElementObject(lynx::RenderObject* render_object);

    virtual ~ElementObject();


protected:

    static void NewElementCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void AppendChildCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void AppendChildrenCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void InsertChildAtIndexCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void RemoveChildByIndexCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void InsertBeforeCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void RemoveChildCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void AddEventListenerCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void RemoveEventListenerCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void SetAttributionCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void SetAttributeCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void SetStyleCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void SetTextCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void GetTextCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void GetNodeTypeCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetParentNodeCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetTagNameCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetOffsetTopCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetOffsetLeftCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetOffsetWidthCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetOffsetHeightCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetScrollWidthCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetScrollHeightCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetScrollTopCallback(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void GetScrollTopCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetScrollLeftCallback(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void GetScrollLeftCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetForceScrollAnimateCallback(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
    static void GetNextSiblingCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetIndexCallback(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void StartCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void StopCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void SetPullViewCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void ClosePullViewCallback(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void GetChildByIndexCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void StopAnimateCallback(const v8::FunctionCallbackInfo <v8::Value> &args);
    static void StartAnimateWithCallbackCallback(const v8::FunctionCallbackInfo <v8::Value> &args);


    base::ScopedPtr<lynx::RenderObject> render_object_;
};
}
#endif
#endif /* ELEMENT_OBJECT_H_ */
