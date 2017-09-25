// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#ifndef LoaderObject_H__
#define LoaderObject_H__

#include "base/weak_ptr.h"
#include "runtime/loader.h"
#include "runtime/v8/v8_js_function.h"
#include "net/url_request_delegate.h"
#include "runtime/v8/object_wrap.h"

namespace jscore {

class V8Runtime;

class LoaderObject : public ObjectWrap, public Loader {
public:
    static void BindingClass(v8::Isolate* isolate, v8::Local <v8::ObjectTemplate> &globalObjectTemplate, V8Context* context, v8::Persistent<v8::FunctionTemplate>& constructor);
    static v8::Local<v8::Value> Create(v8::Isolate* isolate, v8::Persistent<v8::FunctionTemplate>& constructor);

private:
    explicit LoaderObject(V8Context* context);
    virtual ~LoaderObject();

    static void NewLoaderCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void TraceCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void ScriptCallback(const v8::FunctionCallbackInfo <v8::Value> &args);

};
}
#endif /* LoaderObject_H__ */
#endif
