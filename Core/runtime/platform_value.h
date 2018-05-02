// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_PLATFORM_VALUE_H_
#define LYNX_RUNTIME_PLATFORM_VALUE_H_

#if OS_ANDROID

#include "base/android/scoped_java_ref.h"
namespace jscore {
    class PlatformValue {
    public:
        PlatformValue(JNIEnv* env, jobject value) : value_(env, value) {

        }

        ~PlatformValue() {

        }

        jobject value() {
            return value_.Get();
        }

    private:
        base::android::ScopedGlobalJavaRef<jobject> value_;
    };
}

#elif OS_IOS

namespace jscore {
    class PlatformValue {
    };
}

#endif

#endif //LYNX_RUNTIME_PLATFORM_VALUE_H_
