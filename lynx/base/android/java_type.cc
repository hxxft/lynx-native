// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/android/java_type.h"

namespace base {
namespace android {

ScopedLocalJavaRef<jstring> JType::NewString(JNIEnv* env, const char* str) {
    return ScopedLocalJavaRef<jstring>(env, env->NewStringUTF(str));
}

Type JType::getClassType(int retType) {
    Type classReturnType = static_cast<Type>(retType);
    return classReturnType;
}

ScopedLocalJavaRef<jobjectArray> JType::NewObjectArray(JNIEnv* env, int length) {
    EnsureInstance(env, Type::Object);
    return ScopedLocalJavaRef<jobjectArray>
            (env, env->NewObjectArray(length, object_clazz, nullptr));
}

ScopedLocalJavaRef<jobjectArray> JType::NewStringArray(JNIEnv* env, int length) {
    EnsureInstance(env, Type::String);
    return ScopedLocalJavaRef<jobjectArray>
            (env, env->NewObjectArray(length, string_clazz, nullptr));
}

ScopedLocalJavaRef<jobject> JType::NewByte(JNIEnv* env, jbyte value) {
    EnsureInstance(env, Type::Byte);
    jobject result = env->NewObject(byte_clazz, byte_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

ScopedLocalJavaRef<jobject> JType::NewChar(JNIEnv* env, jchar value) {
    EnsureInstance(env, Type::Char);
    jobject result = env->NewObject(char_clazz, char_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

ScopedLocalJavaRef<jobject> JType::NewBoolean(JNIEnv* env, jboolean value) {
    EnsureInstance(env, Type::Boolean);
    jobject result = env->NewObject(boolean_clazz, boolean_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

ScopedLocalJavaRef<jobject> JType::NewShort(JNIEnv* env, jshort value) {
    EnsureInstance(env, Type::Short);
    jobject result = env->NewObject(short_clazz, short_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

ScopedLocalJavaRef<jobject> JType::NewInt(JNIEnv* env, jint value) {
    EnsureInstance(env, Type::Int);
    jobject result = env->NewObject(int_clazz, int_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

ScopedLocalJavaRef<jobject> JType::NewLong(JNIEnv* env, jlong value) {
    EnsureInstance(env, Type::Long);
    jobject result = env->NewObject(long_clazz, long_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

ScopedLocalJavaRef<jobject> JType::NewFloat(JNIEnv* env, jfloat value) {
    EnsureInstance(env, Type::Float);
    jobject result = env->NewObject(float_clazz, float_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

ScopedLocalJavaRef<jobject> JType::NewDouble(JNIEnv* env, jdouble value) {
    EnsureInstance(env, Type::Double);
    jobject result = env->NewObject(double_clazz, double_ctor, value);
    return ScopedLocalJavaRef<jobject>(env, result);
}

jbyte JType::ByteValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Byte);
    return env->GetByteField(value, byte_valueField);
}

jchar JType::CharValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Char);
    return env->GetCharField(value, char_valueField);
}

jboolean JType::BooleanValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Boolean);
    return env->GetBooleanField(value, boolean_valueField);
}

jshort JType::ShortValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Short);
    return env->GetShortField(value, short_valueField);
}

jint JType::IntValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Int);
    return env->GetIntField(value, int_valueField);
}

jlong JType::LongValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Long);
    return env->GetLongField(value, long_valueField);
}

jfloat JType::FloatValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Float);
    return env->GetFloatField(value, float_valueField);
}

jdouble JType::DoubleValue(JNIEnv* env, jobject value) {
    EnsureInstance(env, Type::Double);
    return env->GetDoubleField(value, double_valueField);
}

// LynxArray
ScopedLocalJavaRef<jobject> JType::NewLynxArray(JNIEnv *env, int length) {
    EnsureInstance(env, Type::LynxArray);
    jobject result = env->NewObject(lynx_array_clazz, lynx_array_ctor, length);
    return ScopedLocalJavaRef<jobject>(env, result);
}

void JType::SetLynxArrayElement(JNIEnv *env,
                                jobject array, int index, jobject obj) {

    EnsureInstance(env, Type::LynxArray);
    env->CallVoidMethod(array, lynx_array_set_method, index, obj);
}

ScopedLocalJavaRef<jobject> JType::GetLynxArrayElement(JNIEnv *env, jobject array, int index) {
    EnsureInstance(env, Type::LynxArray);
    jobject result = env->CallObjectMethod(array, lynx_array_get_method, index);
    return ScopedLocalJavaRef<jobject>(env, result);
}

jstring JType::GetLynxArrayElementTypes(JNIEnv *env, jobject array) {
    EnsureInstance(env, Type::LynxArray);
    return (jstring)env->CallObjectMethod(array,
        lynx_array_get_element_type_method);
}

jint JType::GetLynxArrayLength(JNIEnv *env, jobject array) {
    EnsureInstance(env, Type::LynxArray);
    return env->CallIntMethod(array, lynx_array_length_method);
}

// LynxObject
ScopedLocalJavaRef<jobject> JType::NewLynxObject(JNIEnv *env) {
    EnsureInstance(env, Type::LynxObject);
    jobject result = env->NewObject(lynx_object_clazz, lynx_object_ctor);
    return ScopedLocalJavaRef<jobject>(env, result);
}

void JType::SetLynxObjectProperties(JNIEnv *env, jobject jjsobj, jobject array) {
    EnsureInstance(env, Type::LynxObject);
    env->CallVoidMethod(jjsobj, lynx_object_set_properties_method, array);
}

void JType::SetLynxObjectProperty(JNIEnv *env, jobject jjsobj, jobject key, jobject value) {
    EnsureInstance(env, Type::LynxObject);
    env->CallVoidMethod(jjsobj, lynx_object_set_property_method, key, value);
}

ScopedLocalJavaRef<jobject> JType::GetLynxObjectProperties(JNIEnv *env, jobject jjsobj) {
    EnsureInstance(env, Type::LynxObject);
    jobject result = env->CallObjectMethod(jjsobj, lynx_object_get_properties_method);
    return ScopedLocalJavaRef<jobject>(env, result);
}

long JType::GetNativeLynxHolder(JNIEnv* env, jobject holder) {
    EnsureInstance(env, Type::LynxHolder);
    return (long) env->CallLongMethod(holder, lynx_holder_get_native_holder_method);
}

void JType::EnsureInstance(JNIEnv* env, Type type) {
    Init(env, type);
}

bool JType::IsInstanceOf(JNIEnv* env, jobject arg, Type type) {
    if (arg == nullptr) {
        return false;
    }
    // TODO(Provide a fallback mechanism to prevent possible field name changes)
    auto result = false;
    switch (type) {
    case Type::Byte:
        EnsureInstance(env, Type::Byte);
        result = env->IsInstanceOf(arg, byte_clazz);;
        break;
    case Type::Char:
        EnsureInstance(env, Type::Char);
        result = env->IsInstanceOf(arg, char_clazz);
        break;
    case Type::Boolean:
        EnsureInstance(env, Type::Boolean);
        result = env->IsInstanceOf(arg, boolean_clazz);
        break;
    case Type::Short:
        EnsureInstance(env, Type::Short);
        result = env->IsInstanceOf(arg, short_clazz);
        break;
    case Type::Int:
        EnsureInstance(env, Type::Int);
        result = env->IsInstanceOf(arg, int_clazz);
        break;
    case Type::Long:
        EnsureInstance(env, Type::Long);
        result = env->IsInstanceOf(arg, long_clazz);
        break;
    case Type::Float:
        EnsureInstance(env, Type::Float);
        result = env->IsInstanceOf(arg, float_clazz);
        break;
    case Type::Double:
        EnsureInstance(env, Type::Double);
        result = env->IsInstanceOf(arg, double_clazz);
        break;
    case Type::String:
        EnsureInstance(env, Type::String);
        result = env->IsInstanceOf(arg, string_clazz);
        break;
    case Type::LynxArray:
        EnsureInstance(env, Type::LynxArray);
        result = env->IsInstanceOf(arg, lynx_array_clazz);
        break;
    case Type::LynxObject:
        EnsureInstance(env, Type::LynxObject);
        result = env->IsInstanceOf(arg, lynx_object_clazz);
        break;
    default:
        result = false;
        break;
    }
    return result;
}

void JType::Init(JNIEnv* env, Type type) {
    // TODO(Provide a fallback mechanism to prevent possible field name changes)
    switch (type) {
    case Type::Byte:
        if (byte_clazz == nullptr) {
            byte_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Byte"));
            byte_ctor = env->GetMethodID(byte_clazz, "<init>", "(B)V");
            byte_valueField = env->GetFieldID(byte_clazz, "value", "B");
        }
        break;
    case Type::Char:
        if (char_clazz == nullptr) {
            char_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Character"));
            char_ctor = env->GetMethodID(char_clazz, "<init>", "(C)V");
            char_valueField = env->GetFieldID(char_clazz, "value", "C");
        }
        break;
    case Type::Boolean:
        if (boolean_clazz == nullptr) {
            boolean_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Boolean"));
            boolean_ctor = env->GetMethodID(boolean_clazz, "<init>", "(Z)V");
            boolean_valueField = env->GetFieldID(boolean_clazz, "value", "Z");
        }
        break;
    case Type::Short:
        if (short_clazz == nullptr) {
            short_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Short"));
            short_ctor = env->GetMethodID(short_clazz, "<init>", "(S)V");
            short_valueField = env->GetFieldID(short_clazz, "value", "S");
        }
        break;
    case Type::Int:
        if (int_clazz == nullptr) {
            int_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Integer"));
            int_ctor = env->GetMethodID(int_clazz, "<init>", "(I)V");
            int_valueField = env->GetFieldID(int_clazz, "value", "I");
        }
        break;
    case Type::Long:
        if (long_clazz == nullptr) {
            long_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Long"));
            long_ctor = env->GetMethodID(long_clazz, "<init>", "(J)V");
            long_valueField = env->GetFieldID(long_clazz, "value", "J");
        }
        break;
    case Type::Float:
        if (float_clazz == nullptr) {
            float_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Float"));
            float_ctor = env->GetMethodID(float_clazz, "<init>", "(F)V");
            float_valueField = env->GetFieldID(float_clazz, "value", "F");
        }
        break;
    case Type::Double:
        if (double_clazz == nullptr) {
            double_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Double"));
            double_ctor = env->GetMethodID(double_clazz, "<init>", "(D)V");
            double_valueField = env->GetFieldID(double_clazz, "value", "D");
        }
        break;
    case Type::String:
        if (string_clazz == nullptr) {
            string_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/String"));
        }
        break;
    case Type::Object:
        if (object_clazz == nullptr) {
            object_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("java/lang/Object"));
        }
        break;
    case Type::LynxArray:
        if (lynx_array_clazz == nullptr) {
            lynx_array_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("com/lynx/core/base/LynxArray"));

            lynx_array_ctor = env->GetMethodID(lynx_array_clazz, "<init>", "(I)V");

            lynx_array_set_method = env->GetMethodID(
                lynx_array_clazz, "set", "(ILjava/lang/Object;)V");

            lynx_array_get_method = env->GetMethodID(
                lynx_array_clazz, "get", "(I)Ljava/lang/Object;");

            lynx_array_get_element_type_method = env->GetMethodID(
                lynx_array_clazz, "getComponentsType", "()Ljava/lang/String;");

            lynx_array_length_method = env->GetMethodID(
                lynx_array_clazz, "length", "()I");
        }
        break;
    case Type::LynxObject:
        if (lynx_object_clazz == nullptr) {
            lynx_object_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("com/lynx/core/base/LynxObject"));

            lynx_object_ctor = env->GetMethodID(lynx_object_clazz, "<init>", "()V");

            lynx_object_set_properties_method = env->GetMethodID(
                lynx_object_clazz, "setProperties",
                "(Lcom/lynx/core/base/LynxArray;)V");

            lynx_object_set_property_method = env->GetMethodID(
                lynx_object_clazz, "setProperty",
                "(Ljava/lang/Object;Ljava/lang/Object;)V");

            lynx_object_get_properties_method = env->GetMethodID(
                lynx_object_clazz, "getProperties",
                "()Lcom/lynx/core/base/LynxArray;");
        }
        break;
    case Type::LynxHolder:
        if (lynx_holder_clazz == nullptr) {
            lynx_holder_clazz = (jclass)env->NewGlobalRef(
                env->FindClass("com/lynx/core/base/LynxHolder"));

            lynx_holder_get_native_holder_method = env->GetMethodID(
                    lynx_holder_clazz, "getNativeHolder", "()J");
        }
        break;
    }
}

jclass JType::byte_clazz;
jmethodID JType::byte_ctor;
jfieldID JType::byte_valueField;

jclass JType::char_clazz;
jmethodID JType::char_ctor;
jfieldID JType::char_valueField;

jclass JType::boolean_clazz;
jmethodID JType::boolean_ctor;
jfieldID JType::boolean_valueField;

jclass JType::short_clazz;
jmethodID JType::short_ctor;
jfieldID JType::short_valueField;

jclass JType::int_clazz;
jmethodID JType::int_ctor;
jfieldID JType::int_valueField;

jclass JType::long_clazz;
jmethodID JType::long_ctor;
jfieldID JType::long_valueField;

jclass JType::float_clazz;
jmethodID JType::float_ctor;
jfieldID JType::float_valueField;

jclass JType::double_clazz;
jmethodID JType::double_ctor;
jfieldID JType::double_valueField;

jclass JType::string_clazz;
jclass JType::object_clazz;

// LynxArray
jclass JType::lynx_array_clazz;
jmethodID JType::lynx_array_ctor;
jmethodID JType::lynx_array_set_method;
jmethodID JType::lynx_array_get_method;
jmethodID JType::lynx_array_get_element_type_method;
jmethodID JType::lynx_array_length_method;

// LynxObject
jclass JType::lynx_object_clazz;
jmethodID JType::lynx_object_ctor;
jmethodID JType::lynx_object_set_properties_method;
jmethodID JType::lynx_object_set_property_method;
jmethodID JType::lynx_object_get_properties_method;

// LynxObject
jclass JType::lynx_holder_clazz;
jmethodID JType::lynx_holder_get_native_holder_method;

}  // namespace android
}  // namespace base
