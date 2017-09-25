// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_ANDROID_ANDROID_JNI_H_
#define LYNX_BASE_ANDROID_ANDROID_JNI_H_

#include <cstdint>
#include <jni.h>

#include "base/android/scoped_java_ref.h"

namespace base {
namespace android {

void InitVM(JavaVM* vm);

JNIEnv* AttachCurrentThread();

void DetachFromVM();

ScopedLocalJavaRef<jclass> GetClass(JNIEnv* env, const char* class_name);

jclass GetClass(JNIEnv* env, const char* class_name, intptr_t* class_id);

enum MethodType {
    STATIC_METHOD,
    INSTANCE_METHOD,
};

jmethodID GetMethod(JNIEnv* env, jclass clazz,
    MethodType type, const char* method_name, const char* jni_signature);

jmethodID GetMethod(JNIEnv* env, jclass clazz,
    MethodType type, const char* method_name,
    const char* jni_signature, intptr_t* method_id);

bool HasException(JNIEnv* env);
bool ClearException(JNIEnv* env);
void CheckException(JNIEnv* env);
}  // namespace android
}  // namespace base


#endif  // LYNX_BASE_ANDROID_ANDROID_JNI_H_
