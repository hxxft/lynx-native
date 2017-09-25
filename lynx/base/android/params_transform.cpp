// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/android/scoped_java_ref.h"
#include "base/android/params_transform.h"
#include "base/android/jni_helper.h"

#include "ParamsTransform_jni.h"

namespace base {
    namespace android {

        bool ParamsTransform::RegisterJNIUtils(JNIEnv *env) {
            return RegisterNativesImpl(env);
        }

        const std::string ParamsTransform::Transform(JNIEnv *env, jobjectArray array) {
            return JNIHelper::ConvertToString(
                    env,
                    (jstring) Java_ParamsTransform_transformJLS_LJLO(
                            env,
                            array).Get());
        }

        const std::string ParamsTransform::Transform(JNIEnv *env, jobject obj) {
            std::string temp = JNIHelper::ConvertToString(
                    env,
                    (jstring) Java_ParamsTransform_transformJLS_JLO(
                            env,
                            obj).Get());
            return temp;
        }
    }
}  // namespace base
