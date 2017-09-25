// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef LYNX_RUNTIME_ANDROID_LYNX_FUNCTION_OBJECT_H_
#define LYNX_RUNTIME_ANDROID_LYNX_FUNCTION_OBJECT_H_

#include "runtime/base/lynx_function_object.h"
#include "base/android/scoped_java_ref.h"
namespace jscore {
    class LynxFunctionObjectAndroid : public LynxFunctionObject {
    public:
        LynxFunctionObjectAndroid(JNIEnv* env, jobject java_object);
        virtual ~LynxFunctionObjectAndroid();

        static bool RegisterJNIUtils(JNIEnv* env);

    private:
        static base::ScopedPtr<LynxValue> ExecCallback(LynxFunctionObject* object,
                                                     const std::string& name,
                                                     base::ScopedPtr<LynxArray> array);

        base::android::ScopedGlobalJavaRef<jobject> java_object_;
    };
}


#endif //LYNX_RUNTIME_ANDROID_LYNX_FUNCTION_OBJECT_H_
