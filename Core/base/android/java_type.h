// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_ANDROID_JAVA_TYPE_H_
#define LYNX_BASE_ANDROID_JAVA_TYPE_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"

namespace base {
namespace android {

const static char kShortType = 'S';
const static char kIntType = 'I';
const static char kLongType = 'J';
const static char kFloatType = 'F';
const static char kDoubleType = 'D';
const static char kCharType = 'C';
const static char kBooleanType = 'Z';
const static char kByteType = 'B';
const static char kVoidType = 'V';
const static char kArrayFlag = '[';

const static char kStringType = 's';
const static char kObjectType = 'O';

class JType {
 public:
    static ScopedLocalJavaRef<jobject> NewByte(JNIEnv* env, jbyte value);
    static ScopedLocalJavaRef<jobject> NewChar(JNIEnv* env, jchar value);
    static ScopedLocalJavaRef<jobject> NewBoolean(JNIEnv* env, jboolean value);
    static ScopedLocalJavaRef<jobject> NewShort(JNIEnv* env, jshort value);
    static ScopedLocalJavaRef<jobject> NewInt(JNIEnv* env, jint value);
    static ScopedLocalJavaRef<jobject> NewLong(JNIEnv* env, jlong value);
    static ScopedLocalJavaRef<jobject> NewFloat(JNIEnv* env, jfloat value);
    static ScopedLocalJavaRef<jobject> NewDouble(JNIEnv* env, jdouble value);

    static ScopedLocalJavaRef<jstring> NewString(JNIEnv* env, const char* str);

    static jbyte ByteValue(JNIEnv* env, jobject value);
    static jchar CharValue(JNIEnv* env, jobject value);
    static jboolean BooleanValue(JNIEnv* env, jobject value);
    static jshort ShortValue(JNIEnv* env, jobject value);
    static jint IntValue(JNIEnv* env, jobject value);
    static jlong LongValue(JNIEnv* env, jobject value);
    static jfloat FloatValue(JNIEnv* env, jobject value);
    static jdouble DoubleValue(JNIEnv* env, jobject value);

    static ScopedLocalJavaRef<jobjectArray> NewObjectArray(JNIEnv* env, int length);
    static ScopedLocalJavaRef<jdoubleArray> NewDoubleArray(JNIEnv* env, int length);
    static ScopedLocalJavaRef<jobjectArray> NewStringArray(JNIEnv* env, int length);

    static void Init(JNIEnv* env, char type);
    static void ReleaseAll(JNIEnv* env);

 private:

    static void EnsureInstance(JNIEnv* env, char type);

    static jclass byte_clazz;
    static jmethodID byte_ctor;
    static jfieldID byte_valueField;

    static jclass char_clazz;
    static jmethodID char_ctor;
    static jfieldID char_valueField;

    static jclass boolean_clazz;
    static jmethodID boolean_ctor;
    static jfieldID boolean_valueField;

    static jclass short_clazz;
    static jmethodID short_ctor;
    static jfieldID short_valueField;

    static jclass int_clazz;
    static jmethodID int_ctor;
    static jfieldID int_valueField;

    static jclass long_clazz;
    static jmethodID long_ctor;
    static jfieldID long_valueField;

    static jclass float_clazz;
    static jmethodID float_ctor;
    static jfieldID float_valueField;

    static jclass double_clazz;
    static jmethodID double_ctor;
    static jfieldID double_valueField;

    static jclass string_clazz;
    static jclass object_clazz;

};

}  // namespace android
}  // namespace base
#endif //LYNX_BASE_ANDROID_JAVA_TYPE_H_
