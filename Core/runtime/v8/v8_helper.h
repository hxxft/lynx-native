// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#ifndef RUNTIME_V8_HELPER
#define RUNTIME_V8_HELPER

#include <string>

#include "v8.h"
#include "runtime/base/lynx_value.h"

namespace jscore {

class V8Helper {
public:
    static base::ScopedPtr<LynxValue> ConvertToLynxValue(v8::Local<v8::Context> context,
                                                         v8::Local<v8::Value> value);
    static base::ScopedPtr<LynxArray> ConvertToLynxArray(v8::Local<v8::Context> context,
                                                         v8::Local<v8::Array> array);
    static base::ScopedPtr<LynxArray> ConvertToLynxArray(v8::Local<v8::Context> context,
                                                         const v8::FunctionCallbackInfo<v8::Value>& info);
    static base::ScopedPtr<LynxMap> ConvertToLynxMap(v8::Local<v8::Context> context,
                                                     v8::Local<v8::Object> object);
    static base::ScopedPtr<LynxValue> ConvertToLynxFunction(v8::Local<v8::Context> context,
                                                            v8::Local<v8::Function> func);


    static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const std::string &s);
    static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const char* s);
    static v8::Local<v8::Array> ConvertToV8Array(v8::Isolate* isolate, LynxArray* array);
    static v8::Local<v8::Object> ConvertToV8Object(v8::Isolate* isolate, LynxObject* object);
    static v8::Local<v8::Value> ConvertToV8Value(v8::Isolate* isolate, LynxValue* value);
    static v8::Local<v8::Object> ConvertToV8Object(v8::Isolate* isolate, LynxMap* map);

    static std::string ConvertToString(const v8::Local <v8::String> &s);

    inline static v8::Local<v8::Number> ConvertToV8Int(v8::Isolate* isolate, LynxValue* value) {
        return v8::Number::New(isolate, value->data_.i);
    }

    inline static v8::Local<v8::Number> ConvertToV8Long(v8::Isolate* isolate, LynxValue* value) {
        return v8::Number::New(isolate, value->data_.l);
    }

    inline static v8::Local<v8::Number> ConvertToV8Float(v8::Isolate* isolate, LynxValue* value) {
        return v8::Number::New(isolate, value->data_.f);
    }

    inline static v8::Local<v8::Number> ConvertToV8Double(v8::Isolate* isolate, LynxValue* value) {
        return v8::Number::New(isolate, value->data_.d);
    }

    inline static v8::Local<v8::Boolean> ConvertToV8Boolean(v8::Isolate* isolate, LynxValue* value) {
        return v8::Boolean::New(isolate, value->data_.b);
    }

    inline static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, LynxValue* value) {
        return ConvertToV8String(isolate, value->data_.str);
    }

    static v8::Local<v8::String> JsonStringify(v8::Isolate* isolate, const v8::Local<v8::Value>& arg);

};

}

#endif
#endif // RUNTIME_V8_HELPER
