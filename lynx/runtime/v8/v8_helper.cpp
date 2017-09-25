// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include <base/value.h>
#include "base/android/android_jni.h"
#include "runtime/v8/params_transform.h"
#include "runtime/v8/v8_helper.h"

namespace jscore {

std::string V8Helper::ConvertToString(const v8::Local<v8::String>& s) {
    if (s.IsEmpty()) {
        return std::string("");
    }
    if (s->IsNull()) {
        return std::string("Null");
    } else if (!(s->IsString() || s->IsStringObject())) {
        return std::string("value from v8 is not a string or string object");
    } else {
        v8::String::Utf8Value str(s);
        return std::string(*str);
    }
}

v8::Local<v8::String> V8Helper::ConvertToV8String(v8::Isolate* isolate, const std::string& s)
{
    v8::Local<v8::String> str =
        v8::String::NewFromUtf8(isolate, s.c_str(), v8::NewStringType::kNormal).ToLocalChecked();
    return str;
}

v8::Local<v8::String> V8Helper::ConvertToV8String(v8::Isolate* isolate, const char* s)
{
    v8::Local<v8::String> str =
        v8::String::NewFromUtf8(isolate, s, v8::NewStringType::kNormal).ToLocalChecked();
    return str;
}

v8::Local<v8::String> V8Helper::JsonStringify(v8::Isolate* isolate, const v8::Local<v8::Value>& arg) {
    auto context = isolate->GetCurrentContext();
    // Get the global object.
    // Same as using 'global' in Node
    v8::Local<v8::Object> global = v8::Isolate::GetCurrent()->GetCurrentContext()->Global();

    // Get JSON
    // Same as using 'global.JSON'
    v8::Local<v8::Object> JSON = v8::Local<v8::Object>::Cast(
                                     global->Get(context, ConvertToV8String(isolate, "JSON")).ToLocalChecked());

    // Get stringify
    // Same as using 'global.JSON.stringify'
    v8::Local<v8::Function> stringify = v8::Local<v8::Function>::Cast(
                                            JSON->Get(context, ConvertToV8String(isolate, "stringify")).ToLocalChecked());

    // Stringify the object
    // Same as using 'global.JSON.stringify.apply(global.JSON, [ obj ])
    v8::Local<v8::Value> args[] = { arg };

    auto maybeResult = stringify->Call(context, JSON, 1, args);
    if (!maybeResult.IsEmpty()) {
        v8::Local<v8::String> result = v8::Local<v8::String>::Cast(maybeResult.ToLocalChecked());
        return result;
    }
    //if (result.IsEmpty()) {
    //LOGD("JsonStringify", "Exception: %s\n", *exception_str);
    //}


}


#if OS_ANDROID



v8::Local<v8::String> V8Helper::ConvertToV8String(v8::Isolate* isolate, const jchar* data, int length)
{
    return v8::String::NewFromTwoByte(isolate, (const uint16_t*) data, v8::String::kNormalString, length);
}


v8::Local<v8::Array> V8Helper::ConvertToV8Array(v8::Isolate* isolate, JNIEnv* env, const jobjectArray args)
{
    std::string types = ParamsTransform::Transform(env, args);
    int argc = types.size();
    int i = 0;
    v8::Local<v8::Array> arr(v8::Array::New(isolate, argc));
    for (; i < argc; ++i) {
        jobject arg = env->GetObjectArrayElement(args, i);
        v8::Local<v8::Value> v8_arg = ConvertToV8Value(isolate, env, arg, types[i]);
        arr->Set(i, v8_arg);
        env->DeleteLocalRef(arg);
    }
    return arr;
}

v8::Local<v8::String> V8Helper::ConvertToV8String(v8::Isolate* isolate, jchar value)
{
    auto v8_string = V8Helper::ConvertToV8String(isolate, &value, 1);
    return v8_string;
}

v8::Local<v8::String> V8Helper::ConvertToV8String(v8::Isolate* isolate, JNIEnv* env, jstring value)
{
    if (value == nullptr)
    {
        return ConvertToV8String(isolate, "");
    }
    auto chars = env->GetStringChars(value, NULL);
    auto length = env->GetStringLength(value);
    auto v8_string = ConvertToV8String(isolate, chars, length);
    env->ReleaseStringChars(value, chars);

    return v8_string;
}

v8::Local<v8::Value> V8Helper::ConvertToV8Value(v8::Isolate* isolate, JNIEnv* env, const jobject jobj, char type) {
    v8::Local<v8::Value> v8_obj;
    switch (type) {
    case 'I':
        v8_obj = ConvertToV8Int(isolate, env, jobj);
        break;
    case 'B':
        v8_obj = ConvertToV8Byte(isolate, env, jobj);
        break;
    case 'S':
        v8_obj = ConvertToV8Short(isolate, env, jobj);
        break;
    case 'J':
        v8_obj = ConvertToV8Long(isolate, env, jobj);
        break;
    case 'F':
        v8_obj = ConvertToV8Float(isolate, env, jobj);
        break;
    case 'D':
        v8_obj = ConvertToV8Double(isolate, env, jobj);
        break;
    case 'C':
        v8_obj = ConvertToV8Char(isolate, env, jobj);
        break;
    case 'Z':
        v8_obj = ConvertToV8Boolean(isolate, env, jobj);
        break;
    case 's': // String
        v8_obj = ConvertToV8String(isolate, env, (jstring) jobj);
        break;
    case 'b': // LynxArray
        v8_obj = ConvertToV8Array(isolate, env, jobj);
        break;
    case 'c': // JSObjsect
        v8_obj = ConvertToV8Object(isolate, env, jobj);
        break;
    case 'V':
        v8_obj = v8::Local<v8::Value>(v8::Undefined(isolate));
        break;
    default:
        v8_obj = v8::Local<v8::Value>(v8::Null(isolate));
        break;
    }
    return v8_obj;
}

v8::Local<v8::Object> V8Helper::ConvertToV8Object(v8::Isolate* isolate, JNIEnv* env, const jobject obj) {
    auto v8_obj = v8::Object::New(isolate);
    jobject properties_array_java = JType::GetJSObjectProperties(env, obj);
    if (properties_array_java != NULL) {
        auto properties_prray_js = ConvertToV8Array(isolate, env, properties_array_java);
        env->DeleteLocalRef(properties_array_java);
        int length = properties_prray_js->Length();
        for (int i = 0; i < length; i += 2) {
            v8_obj->Set(properties_prray_js->Get(i), properties_prray_js->Get(i + 1));
        }
    }
    return v8_obj;
}

v8::Local<v8::Array> V8Helper::ConvertToV8Array(v8::Isolate* isolate, JNIEnv* env, const jobject array) {
    int length = (int)JType::GetJSArrayLength(env, array);
    jstring types_j = JType::GetJSArrayElementTypes(env, array);
    const char* types_c = env->GetStringUTFChars(types_j, JNI_FALSE);
    std::string types(types_c);
    v8::Local<v8::Array> v8_array(v8::Array::New(isolate, length));
    for (int i = 0; i < length; i++) {
        jobject java_obj = JType::GetJSArrayElement(env, array, i);
        auto v8_obj = ConvertToV8Value(isolate, env, java_obj, types[i]);
        v8_array->Set(i, v8_obj);
        env->DeleteLocalRef(java_obj);
    }
    env->ReleaseStringUTFChars(types_j, types_c);
    env->DeleteLocalRef(types_j);
    return v8_array;
}

#endif
}
#endif
