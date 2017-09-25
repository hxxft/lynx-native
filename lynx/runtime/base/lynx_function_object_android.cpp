// Copyright 2017 The Lynx Authors. All rights reserved.

#include <base/android/jni_helper.h>
#include <assert.h>
#include "base/android/android_jni.h"
#include "lynx_function_object_android.h"
#include "gen/LynxFunctionObject_jni.h"

namespace jscore {

    LynxFunctionObjectAndroid::LynxFunctionObjectAndroid(JNIEnv* env, jobject java_object)
            : java_object_(env, java_object) {
        base::android::ScopedLocalJavaRef<jobject> scoped_java_object
                = Java_LynxFunctionObject_getMethodNameArray(env, java_object);
        base::ScopedPtr<LynxArray> array = LynxValue::MakeArrayScoped(
                base::android::JNIHelper::ConvertToLynxArray(env, scoped_java_object.Get()));
        for (int i = 0; i < array->Size(); ++i) {
            LynxValue* value = array->Get(i);
            assert(value->type_ == LynxValue::Type::VALUE_STRING);
            RegisterMethodCallback(value->data_.str, ExecCallback);
        }
    }

    LynxFunctionObjectAndroid::~LynxFunctionObjectAndroid() {

    }

    bool LynxFunctionObjectAndroid::RegisterJNIUtils(JNIEnv* env) {
        return RegisterNativesImpl(env);
    }

    base::ScopedPtr<LynxValue>
    LynxFunctionObjectAndroid::ExecCallback(LynxFunctionObject* object,
                                          const std::string& name,
                                          base::ScopedPtr<LynxArray> array) {

        JNIEnv* env = base::android::AttachCurrentThread();
        jobject java_object = static_cast<LynxFunctionObjectAndroid*>(object)
                ->java_object_.Get();
        base::android::ScopedLocalJavaRef<jstring> java_name
                = base::android::JNIHelper::ConvertToJNIString(
                        env,
                        const_cast<std::string &>(name));
        base::android::ScopedLocalJavaRef<jobjectArray> args =
                base::android::JNIHelper::ConvertToJNIObjectArray(env, array.Get());
        base::android::ScopedLocalJavaRef<jobject> result
                = Java_LynxFunctionObject_exec(env,
                                             java_object,
                                               (jstring) java_name.Get(),
                                               (jobjectArray) args.Get());
        jscore::LynxValue* js_result = base::android::JNIHelper::ConvertToLynxValue(env,
                                                                                    result.Get());
        return LynxValue::MakeValueScoped(js_result);
    }

}