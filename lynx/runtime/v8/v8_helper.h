// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#ifndef V8Helper_H__
#define V8Helper_H__

#include <string>
#include <jni.h>

#include "v8.h"

#include "base/android/java_type.h"
#include "base/value.h"

using namespace base::android;
using v8::Context;
using v8::Local;

namespace jscore {

class V8Helper {
public:
    static std::string ConvertToString(const v8::Local <v8::String> &s);
    static v8::Local <v8::String> ConvertToV8String(v8::Isolate* isolate, const std::string &s);
    static v8::Local <v8::String> ConvertToV8String(v8::Isolate* isolate, const char* s);
    static v8::Local<v8::String> JsonStringify(v8::Isolate* isolate, const v8::Local<v8::Value>& arg);

    static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, const jchar* data, int length);
    static v8::Local<v8::Array> ConvertToV8Array(v8::Isolate* isolate, JNIEnv* env, const jobjectArray args);
    static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, jchar value);
    static v8::Local<v8::String> ConvertToV8String(v8::Isolate* isolate, JNIEnv* env, jstring value);

    inline static bool ConvertToBoolean(const v8::Local<v8::Value>& jsValue) {
        return jsValue->BooleanValue();
    }

    inline static v8::Local<v8::Number> ConvertToV8Int(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        int value = JType::IntValue(env, jobj);
        return v8::Number::New(isolate, value);
    }

    inline static v8::Local<v8::Number> ConvertToV8Int(v8::Isolate* isolate, jint value) {
        return v8::Number::New(isolate, value);
    }

    inline static v8::Local<v8::Number> ConvertToV8Byte(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        return v8::Number::New(isolate, JType::ByteValue(env, jobj));
    }

    inline static v8::Local<v8::Number> ConvertToV8Byte(v8::Isolate* isolate, jbyte value) {
        return v8::Number::New(isolate, value);
    }

    inline static v8::Local<v8::Number> ConvertToV8Short(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        return v8::Number::New(isolate, JType::ShortValue(env, jobj));
    }

    inline static v8::Local<v8::Number> ConvertToV8Short(v8::Isolate* isolate, jshort value) {
        return v8::Number::New(isolate, value);
    }

    inline static v8::Local<v8::Number> ConvertToV8Long(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        return v8::Number::New(isolate, JType::LongValue(env, jobj));
    }

    inline static v8::Local<v8::Number> ConvertToV8Long(v8::Isolate* isolate, jlong value) {
        return v8::Number::New(isolate, value);
    }

    inline static v8::Local<v8::Number> ConvertToV8Float(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        return v8::Number::New(isolate, JType::FloatValue(env, jobj));
    }

    inline static v8::Local<v8::Number> ConvertToV8Float(v8::Isolate* isolate, jfloat value) {
        return v8::Number::New(isolate, value);
    }

    inline static v8::Local<v8::Number> ConvertToV8Double(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        return v8::Number::New(isolate, JType::DoubleValue(env, jobj));
    }

    inline static v8::Local<v8::Number> ConvertToV8Double(v8::Isolate* isolate, jdouble value) {
        return v8::Number::New(isolate, value);
    }

    inline static v8::Local<v8::String> ConvertToV8Char(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        jchar jc = JType::CharValue(env, jobj);
        auto v8Char = V8Helper::ConvertToV8String(isolate, &jc, 1);
        return v8Char;
    }

    inline static v8::Local<v8::String> ConvertToV8Char(v8::Isolate* isolate, jchar value) {
        auto v8Char = V8Helper::ConvertToV8String(isolate, &value, 1);
        return v8Char;
    }

    inline static v8::Local<v8::Boolean> ConvertToV8Boolean(v8::Isolate* isolate, JNIEnv* env, jobject jobj) {
        return v8::Boolean::New(isolate, JType::BooleanValue(env, jobj));
    }

    inline static v8::Local<v8::Boolean> ConvertToV8Boolean(v8::Isolate* isolate, jboolean value) {
        return v8::Boolean::New(isolate, value);
    }

    static v8::Local<v8::Array> ConvertToV8Array(v8::Isolate* isolate, JNIEnv* env, const jobject array);

    static v8::Local<v8::Object> ConvertToV8Object(v8::Isolate* isolate, JNIEnv* env, const jobject obj);

    static v8::Local<v8::Value> ConvertToV8Value(v8::Isolate* isolate, JNIEnv* env, const jobject jobj, char type);
};

}

#endif
#endif /* V8Helper_h */
