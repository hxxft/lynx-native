// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_ANDROID_RESULT_CALLBACK_H_
#define LYNX_RUNTIME_ANDROID_RESULT_CALLBACK_H_

#include "runtime/runtime.h"
#include "base/android/scoped_java_ref.h"

namespace jscore {
    class ResultCallbackAndroid : public ResultCallback {
    public:
        static bool RegisterJNIUtils(JNIEnv* env);

        ResultCallbackAndroid(JNIEnv* env, jobject callback) : ref_(env, callback) {
        }

        virtual void OnReceiveResult(const std::string &result) const;

    private:
        base::android::ScopedGlobalJavaRef<jobject> ref_;
    };
}


#endif //LYNX_RUNTIME_ANDROID_RESULT_CALLBACK_H_
