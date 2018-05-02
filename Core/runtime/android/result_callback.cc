// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/android/jni_helper.h"
#include "runtime/android/result_callback.h"
#include "ResultCallback_jni.h"

namespace jscore {

    void ResultCallbackAndroid::OnReceiveResult(const std::string &result) const {
        JNIEnv* env = base::android::AttachCurrentThread();
        jstring jstr = base::android::JNIHelper::ConvertToJNIString(env, (string &) result).Release();
        Java_ResultCallback_onReceiveResult(env, ref_.Get(), jstr);
    }

    bool ResultCallbackAndroid::RegisterJNIUtils(JNIEnv* env) {
        return RegisterNativesImpl(env);
    }

}