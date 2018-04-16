// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef ANDROID_METHOD_REGISTER_H
#define ANDROID_METHOD_REGISTER_H

#include <jni.h>

namespace lynx {
        class ElementRegisterUtil {
        public:
            static bool RegisterJNIUtils(JNIEnv *env);
        };
}
#endif //ANDROID_METHOD_REGISTER_H_H
