// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/android/lynx_java_type.h"

namespace base {
namespace android {
// LynxArray
ScopedLocalJavaRef<jobject> LxJType::NewLynxArray(JNIEnv* env, int length) {
  EnsureInstance(env, kLynxArrayType);
  jobject result = env->NewObject(lynx_array_clazz, lynx_array_ctor, length);
  return ScopedLocalJavaRef<jobject>(env, result);
}

void LxJType::SetLynxArrayElement(JNIEnv* env,
                                  jobject array,
                                  int index,
                                  jobject obj) {
  EnsureInstance(env, kLynxArrayType);
  env->CallVoidMethod(array, lynx_array_set_method, index, obj);
}

ScopedLocalJavaRef<jobject> LxJType::GetLynxArrayElement(JNIEnv* env,
                                                         jobject array,
                                                         int index) {
  EnsureInstance(env, kLynxArrayType);
  jobject result = env->CallObjectMethod(array, lynx_array_get_method, index);
  return ScopedLocalJavaRef<jobject>(env, result);
}

jstring LxJType::GetLynxArrayElementTypes(JNIEnv* env, jobject array) {
  EnsureInstance(env, kLynxArrayType);
  return (jstring)env->CallObjectMethod(array,
                                        lynx_array_get_element_type_method);
}

jint LxJType::GetLynxArrayLength(JNIEnv* env, jobject array) {
  EnsureInstance(env, kLynxArrayType);
  return env->CallIntMethod(array, lynx_array_length_method);
}

// LynxObject
ScopedLocalJavaRef<jobject> LxJType::NewLynxObject(JNIEnv* env) {
  EnsureInstance(env, kLynxObjectType);
  jobject result = env->NewObject(lynx_object_clazz, lynx_object_ctor);
  return ScopedLocalJavaRef<jobject>(env, result);
}

void LxJType::SetLynxObjectProperties(JNIEnv* env,
                                      jobject jjsobj,
                                      jobject array) {
  EnsureInstance(env, kLynxObjectType);
  env->CallVoidMethod(jjsobj, lynx_object_set_properties_method, array);
}

void LxJType::SetLynxObjectProperty(JNIEnv* env,
                                    jobject jjsobj,
                                    jobject key,
                                    jobject value) {
  EnsureInstance(env, kLynxObjectType);
  env->CallVoidMethod(jjsobj, lynx_object_set_property_method, key, value);
}

ScopedLocalJavaRef<jobject> LxJType::GetLynxObjectProperties(JNIEnv* env,
                                                             jobject jjsobj) {
  EnsureInstance(env, kLynxObjectType);
  jobject result =
      env->CallObjectMethod(jjsobj, lynx_object_get_properties_method);
  return ScopedLocalJavaRef<jobject>(env, result);
}

long LxJType::GetNativeLynxHolder(JNIEnv* env, jobject holder) {
  EnsureInstance(env, kLynxHolderType);
  return (long)env->CallLongMethod(holder,
                                   lynx_holder_get_native_holder_method);
}

void LxJType::EnsureInstance(JNIEnv* env, char type) {
  Init(env, type);
}

void LxJType::Init(JNIEnv* env, char type) {
  switch (type) {
    case kLynxArrayType:
      if (lynx_array_clazz == nullptr) {
        lynx_array_clazz = (jclass)env->NewGlobalRef(
            env->FindClass("com/lynx/core/base/LynxArray"));

        lynx_array_ctor = env->GetMethodID(lynx_array_clazz, "<init>", "(I)V");

        lynx_array_set_method =
            env->GetMethodID(lynx_array_clazz, "set", "(ILjava/lang/Object;)V");

        lynx_array_get_method =
            env->GetMethodID(lynx_array_clazz, "get", "(I)Ljava/lang/Object;");

        lynx_array_get_element_type_method = env->GetMethodID(
            lynx_array_clazz, "getComponentsType", "()Ljava/lang/String;");

        lynx_array_length_method =
            env->GetMethodID(lynx_array_clazz, "length", "()I");
      }
      break;
    case kLynxObjectType:
      if (lynx_object_clazz == nullptr) {
        lynx_object_clazz = (jclass)env->NewGlobalRef(
            env->FindClass("com/lynx/core/base/LynxObject"));

        lynx_object_ctor = env->GetMethodID(lynx_object_clazz, "<init>", "()V");

        lynx_object_set_properties_method =
            env->GetMethodID(lynx_object_clazz, "setProperties",
                             "(Lcom/lynx/core/base/LynxArray;)V");

        lynx_object_set_property_method =
            env->GetMethodID(lynx_object_clazz, "setProperty",
                             "(Ljava/lang/Object;Ljava/lang/Object;)V");

        lynx_object_get_properties_method =
            env->GetMethodID(lynx_object_clazz, "getProperties",
                             "()Lcom/lynx/core/base/LynxArray;");
      }
      break;
    case kLynxHolderType:
      if (lynx_holder_clazz == nullptr) {
        lynx_holder_clazz = (jclass)env->NewGlobalRef(
            env->FindClass("com/lynx/core/base/LynxHolder"));

        lynx_holder_get_native_holder_method =
            env->GetMethodID(lynx_holder_clazz, "getNativeHolder", "()J");
      }
      break;
    default:
      break;
  }
}

// LynxArray
jclass LxJType::lynx_array_clazz;
jmethodID LxJType::lynx_array_ctor;
jmethodID LxJType::lynx_array_set_method;
jmethodID LxJType::lynx_array_get_method;
jmethodID LxJType::lynx_array_get_element_type_method;
jmethodID LxJType::lynx_array_length_method;

// LynxObject
jclass LxJType::lynx_object_clazz;
jmethodID LxJType::lynx_object_ctor;
jmethodID LxJType::lynx_object_set_properties_method;
jmethodID LxJType::lynx_object_set_property_method;
jmethodID LxJType::lynx_object_get_properties_method;

// LynxObject
jclass LxJType::lynx_holder_clazz;
jmethodID LxJType::lynx_holder_get_native_holder_method;

}  // namespace android
}  // namespace base