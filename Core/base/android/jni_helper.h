// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef LYNX_BASE_ANDROID_JNI_HELPER_H_
#define LYNX_BASE_ANDROID_JNI_HELPER_H_

#include <string>
#include <jni.h>

#include "base/scoped_ptr.h"
#include "base/android/scoped_java_ref.h"
#include "base/android/lynx_java_type.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_holder.h"

namespace base {
    namespace android {
        class JNIHelper {
        public:
            static base::android::ScopedLocalJavaRef<jstring> ConvertToJNIString
                    (JNIEnv* env, std::string& value) {
                jstring str = env->NewStringUTF(value.c_str());
                return base::android::ScopedLocalJavaRef<jstring>(env, str);
            }

            static base::android::ScopedLocalJavaRef<jstring> ConvertToJNIString
                    (JNIEnv* env, std::string&& value) {
                jstring str = env->NewStringUTF(value.c_str());
                return base::android::ScopedLocalJavaRef<jstring>(env, str);
            }

            static std::string ConvertToString(JNIEnv* env, jstring value);

            static base::android::ScopedLocalJavaRef<jobject> ConvertToJNIObject
                    (JNIEnv *env, jscore::LynxValue*);
            static base::android::ScopedLocalJavaRef<jobjectArray> ConvertToJNIObjectArray
                    (JNIEnv* env, jscore::LynxArray*);
            static base::android::ScopedLocalJavaRef<jobject> ConvertToJNIArray
                    (JNIEnv* env, jscore::LynxArray*);
            static base::android::ScopedLocalJavaRef<jobject> ConvertToJNIObject
                    (JNIEnv* env, jscore::LynxObject*);

            inline static int ConvertToInt(JNIEnv* env, jobject jobj) {
                int value = JType::IntValue(env, jobj);
                return value;
            }

            inline static long ConvertToLong(JNIEnv* env, jobject jobj) {
                long value = (long)JType::LongValue(env, jobj);
                return value;
            }

            inline static float ConvertToFloat(JNIEnv* env, jobject jobj) {
                float value = (float) JType::FloatValue(env, jobj);
                return value;
            }

            inline static double ConvertToDouble(JNIEnv* env, jobject jobj) {
                double value = (double) JType::DoubleValue(env, jobj);
                return value;
            }

            inline static bool ConvertToBoolean(JNIEnv* env, jobject jobj) {
                jboolean value = JType::BooleanValue(env, jobj);
                return (bool)(value == JNI_TRUE);
            }

            static base::ScopedPtr<jscore::LynxObject> ConvertToLynxObject(JNIEnv *env,
                                                           jobject value);
            static base::ScopedPtr<jscore::LynxFunctionObject> ConvertToLynxFunctionObject(
                    JNIEnv *env,
                    jobject value);
            static base::ScopedPtr<jscore::LynxValue> ConvertToLynxHolder(
                    JNIEnv *env,
                    jobject value);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jobject value);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jobject value,
                                                         char type);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jintArray java_array);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jshortArray java_array);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jlongArray java_array);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jfloatArray java_array);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jdoubleArray java_array);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jbooleanArray java_array);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         jbyteArray java_array);
            static base::ScopedPtr<jscore::LynxArray> ConvertToLynxArray(JNIEnv *env,
                                                         const jobjectArray args);
            static base::ScopedPtr<jscore::LynxValue> ConvertToLynxValue(JNIEnv *env,
                                                         jobject value,
                                                         const std::string &type);
            static base::ScopedPtr<jscore::LynxValue> ConvertToLynxValue(JNIEnv *env,
                                                                         jobject java_obj);


        private:
            static base::ScopedPtr<jscore::LynxValue> ConvertToLynxValue(JNIEnv *env,
                                                         jobject value,
                                                         char first,
                                                         char second);

        };
    }
}

#endif  // LYNX_BASE_ANDROID_JNI_HELPER_H_
