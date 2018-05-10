// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef LYNX_RUNTIME_ANDROID_LYNX_OBJECT_ANDROID_H_
#define LYNX_RUNTIME_ANDROID_LYNX_OBJECT_ANDROID_H_

#include "base/android/scoped_java_ref.h"
#include "runtime/base/lynx_object_platform.h"

namespace jscore {
    class JSContext;

    class LynxObjectAndroid : public LynxObjectPlatform {
    public:

        LynxObjectAndroid(JNIEnv* env, jobject java_object);
        virtual ~LynxObjectAndroid();

        base::ScopedPtr<LynxValue> Exec(const std::string& name,
                                        base::ScopedPtr<LynxArray> array);

        static bool RegisterJNIUtils(JNIEnv* env);

    private:
        base::android::ScopedGlobalJavaRef<jobject> java_object_;
    };
}


#endif //LYNX_RUNTIME_ANDROID_LYNX_OBJECT_ANDROID_H_
