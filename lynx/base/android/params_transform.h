// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_ANDROID_PARAMS_TRANSFORM_H_
#define LYNX_BASE_ANDROID_PARAMS_TRANSFORM_H_

#include <string>

#include "base/android/android_jni.h"

namespace base {
    namespace android {
        class ParamsTransform {
        public:
            static bool RegisterJNIUtils(JNIEnv *env);

            static const std::string Transform(JNIEnv *env, jobjectArray array);

            static const std::string Transform(JNIEnv *env, jobject obj);

        private:
            static jclass params_transform_class;
            static jmethodID transform_param_method;
            static jmethodID transform_params_method;
        };
    }
}  // namespace base

#endif  // LYNX_BASE_ANDROID_PARAMS_TRANSFORM_H_
