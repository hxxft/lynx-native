// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/android/lynx_object_android.h"

#include <assert.h>
#include "base/android/android_jni.h"
#include "base/android/jni_helper.h"
#include "runtime/js/defines.h"
#include "runtime/js/js_context.h"
#include "gen/LynxObject_jni.h"

namespace jscore {

    DEFINE_GROUP_METHOD_CALLBACK(LynxObjectAndroid, Exec)

    LynxObjectAndroid::LynxObjectAndroid(JNIEnv* env, jobject java_object)
            : java_object_(env, java_object) {
        base::android::ScopedLocalJavaRef<jobject> scoped_java_object
                = Java_LynxObject_getMethodNameArray(env, java_object);
        base::ScopedPtr<LynxArray> array = base::android::JNIHelper::ConvertToLynxArray(env, scoped_java_object.Get());
        for (int i = 0; i < array->Size(); ++i) {
            LynxValue* value = array->Get(i);
            assert(value->type_ == LynxValue::Type::VALUE_STRING);
            RegisterMethodCallback(value->data_.str, &METHOD_CALLBACK(Exec));
        }
    }

    LynxObjectAndroid::~LynxObjectAndroid() {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_LynxObject_destroy(env, java_object_.Get());
    }

    bool LynxObjectAndroid::RegisterJNIUtils(JNIEnv* env) {
        return RegisterNativesImpl(env);
    }

    base::ScopedPtr<LynxValue> LynxObjectAndroid::Exec(const std::string &name,
                                                        base::ScopedPtr<LynxArray> array) {
        JNIEnv* env = base::android::AttachCurrentThread();
        jobject java_object = java_object_.Get();
        base::android::ScopedLocalJavaRef<jstring> java_name
                = base::android::JNIHelper::ConvertToJNIString(
                        env,
                        const_cast<std::string &>(name));
        base::android::ScopedLocalJavaRef<jobjectArray> args =
                base::android::JNIHelper::ConvertToJNIObjectArray(env, array.Get());
        base::android::ScopedLocalJavaRef<jobject> result
                = Java_LynxObject_exec(env,
                                               java_object,
                                               (jstring) java_name.Get(),
                                               (jobjectArray) args.Get());
        return base::android::JNIHelper::ConvertToLynxValue(env,
                result.Get());
    }

}