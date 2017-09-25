// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_function_object_android.h"
#include "base/android/jni_helper.h"

#include "base/debug/memory_debug.h"
#include "base/android/params_transform.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_object_template.h"
#include "runtime/platform_value.h"

namespace base {
    namespace android {
        std::string JNIHelper::ConvertToString(JNIEnv* env, jstring jstr)
        {
            const char *str = env->GetStringUTFChars(jstr, JNI_FALSE);
            return str;
        }

        base::android::ScopedLocalJavaRef<jobject> JNIHelper::ConvertToJNIObject(JNIEnv *env,
                                              jscore::LynxValue *value) {
            base::android::ScopedLocalJavaRef<jobject> obj;
            switch (value->type_) {
                case jscore::LynxValue::VALUE_INT:
                    obj = JType::NewInt(env, value->data_.i);
                    break;
                case jscore::LynxValue::VALUE_LONG:
                    obj = JType::NewLong(env, value->data_.l);
                    break;
                case jscore::LynxValue::VALUE_BOOL:
                    obj = JType::NewBoolean(env, value->data_.b);
                    break;
                case jscore::LynxValue::VALUE_FLOAT:
                    obj = JType::NewFloat(env, value->data_.f);
                    break;
                case jscore::LynxValue::VALUE_DOUBLE:
                    obj = JType::NewDouble(env, value->data_.d);
                    break;
                case jscore::LynxValue::VALUE_STRING:
                    obj = base::android::ScopedLocalJavaRef<jobject>
                            (env, JType::NewString(env, value->data_.str).Release());
                    break;
                case jscore::LynxValue::VALUE_LYNX_ARRAY:
                    obj = base::android::ScopedLocalJavaRef<jobject>
                            (env, ConvertToJNIArray(env, value->data_.lynx_array).Release());
                    break;
                default:
                    break;
            }
            return obj;
        }

        base::android::ScopedLocalJavaRef<jobjectArray> JNIHelper::ConvertToJNIObjectArray
                (JNIEnv* env, jscore::LynxArray* array) {
            base::android::ScopedLocalJavaRef<jobjectArray> java_object_array =
                    base::android::JType::NewObjectArray(env, array->Size());
            for (int i = 0; i < array->Size(); ++i) {
                base::android::ScopedLocalJavaRef<jobject> java_object
                        = ConvertToJNIObject(env, array->Get(i));
                env->SetObjectArrayElement(java_object_array.Get(), i, java_object.Get());
            }
            return java_object_array;
        }

        base::android::ScopedLocalJavaRef<jobject> JNIHelper::ConvertToJNIArray
                (JNIEnv* env, jscore::LynxArray* array) {
            ScopedLocalJavaRef<jobject> java_object_array =
                    base::android::JType::NewLynxArray(env, array->Size());
            for (int i = 0; i < array->Size(); ++i) {
                base::android::ScopedLocalJavaRef<jobject> java_object
                        = ConvertToJNIObject(env, array->Get(i));
                base::android::JType::SetLynxArrayElement(env,
                                                          java_object_array.Get(),
                                                          i,
                                                          java_object.Get());
            }
            return java_object_array;
        }

        jscore::LynxObject* JNIHelper::ConvertToLynxObject(JNIEnv *env, jobject obj) {
            base::android::ScopedLocalJavaRef<jobject> properties_array_java
                    = base::android::JType::GetLynxObjectProperties(env, obj);
            jscore::LynxObject* js_obj = lynx_new jscore::LynxObject();
            if (!properties_array_java.IsNull()) {
                base::ScopedPtr<jscore::LynxArray> js_properties_array(ConvertToLynxArray(env, properties_array_java.Get()));
                for (int i = 0; i < js_properties_array->Size(); i += 2) {
                    base::ScopedPtr<jscore::LynxValue> str_ref(js_properties_array->Get(i));
                    js_obj->Set(str_ref->data_.str, js_properties_array->Get(i + 1));
                }
                js_properties_array->Release();
            }
            return js_obj;
        }

        jscore::LynxFunctionObject* JNIHelper::ConvertToLynxFunctionObject(JNIEnv *env,
                                                                           jobject value) {
            jscore::LynxFunctionObjectAndroid* function_object
                    = lynx_new jscore::LynxFunctionObjectAndroid(env, value);
            return function_object;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jobject java_array) {
            int length = (int) base::android::JType::GetLynxArrayLength(env, java_array);
            jstring types_j = base::android::JType::GetLynxArrayElementTypes(env, java_array);
            const char* types_c = env->GetStringUTFChars(types_j, JNI_FALSE);
            std::string types(types_c);

            jscore::LynxArray* lynx_array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                base::android::ScopedLocalJavaRef<jobject> java_obj
                        = base::android::JType::GetLynxArrayElement(env, java_array, i);
                jscore::LynxValue* object = ConvertToLynxValue(env,
                                                               java_obj.Get(),
                                                               types[i],
                                                               types[i] == base::android::kArrayFlag
                                                               ?
                                                               types[i++]
                                                               : base::android::kVoidType);
                lynx_array->Push(object);
            }
            env->ReleaseStringUTFChars(types_j, types_c);
            env->DeleteLocalRef(types_j);
            return lynx_array;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, const jobjectArray args) {
            std::string types = base::android::ParamsTransform::Transform(env, args);
            int length = types.size();
            int i = 0;
            jscore::LynxArray* values = lynx_new jscore::LynxArray();
            for (; i < length; ++i) {
                base::android::ScopedLocalJavaRef<jobject> arg
                        (env, env->GetObjectArrayElement(args, i));
                jscore::LynxValue *object = ConvertToLynxValue(env,
                                                               arg.Get(),
                                                               types[i],
                                                               types[i] == base::android::kArrayFlag
                                                               ?
                                                               types[i++]
                                                               : base::android::kVoidType);
                values->Push(object);
            }
            return values;
        }

        jscore::LynxValue* JNIHelper::ConvertToLynxValue(JNIEnv *env, jobject java_obj) {
            if (java_obj == 0) {
                return NULL;
            }
            std::string type = ParamsTransform::Transform(env, java_obj);
            return ConvertToLynxValue(env, java_obj, type);
        }

        jscore::LynxValue* JNIHelper::ConvertToLynxValue(JNIEnv *env,
                                                         jobject java_obj,
                                                         char first_char,
                                                         char second_char) {
            jscore::LynxValue* value = 0;
            switch (first_char) {
                case base::android::kIntType:
                    value = jscore::LynxValue::MakeInt(ConvertToInt(env, java_obj));
                    break;
                case base::android::kLongType:
                    value = jscore::LynxValue::MakeLong(ConvertToLong(env, java_obj));
                    break;
                case base::android::kFloatType:
                    value = jscore::LynxValue::MakeFloat(ConvertToFloat(env, java_obj));
                    break;
                case base::android::kDoubleType:
                    value = jscore::LynxValue::MakeDouble(ConvertToDouble(env, java_obj));
                    break;
                case base::android::kBooleanType:
                    value = jscore::LynxValue::MakeBool(ConvertToBoolean(env, java_obj));
                    break;
                case base::android::kStringType: // String
                    value = jscore::LynxValue::MakeString(ConvertToString(env, (jstring) java_obj));
                    break;
                case base::android::kLynxArrayType: // LynxArray
                    value = ConvertToLynxArray(env, java_obj);
                    break;
                case base::android::kLynxObjectType: // LynxObject
                    value = ConvertToLynxObject(env, java_obj);
                    break;
                case base::android::kLynxFunctionObjectType: // LynxFunctionObject
                    value = jscore::LynxValue::MakeFunctionObject(
                            ConvertToLynxFunctionObject(env, java_obj));
                    break;
                case base::android::kArrayFlag:
                    value = ConvertToLynxArray(env, java_obj, second_char);
                    break;
                case base::android::kVoidType:
                default:
                    break;
            }
            return value;
        }

        jscore::LynxValue* JNIHelper::ConvertToLynxValue(JNIEnv *env,
                                                         jobject java_obj,
                                                         const std::string &type) {
            char first_char = type[0];
            char second_char = type.length() > 1? type[1] : base::android::kVoidType;
            return ConvertToLynxValue(env, java_obj, first_char, second_char);
        }

        jscore::LynxArray *JNIHelper::ConvertToLynxArray(JNIEnv *env,
                                                         jobject value,
                                                         char type) {
            switch (type) {
                case base::android::kIntType:
                    return ConvertToLynxArray(env, (jintArray) value);
                case base::android::kLongType:
                    return ConvertToLynxArray(env, (jlongArray) value);
                case base::android::kFloatType:
                    return ConvertToLynxArray(env, (jfloatArray) value);
                case base::android::kDoubleType:
                    return ConvertToLynxArray(env, (jdoubleArray) value);
                case base::android::kBooleanType:
                    return ConvertToLynxArray(env, (jbooleanArray) value);
                default:
                    break;
            }
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            int length = env->GetArrayLength((jarray) value);

            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* element = NULL;
                base::android::ScopedLocalJavaRef<jobject> temp
                        (env, env->GetObjectArrayElement((jobjectArray) value, i));
                switch (type) {
                    case base::android::kStringType: // String
                        element = jscore::LynxValue::MakeString(ConvertToString(env,
                                                                              (jstring) temp.Get()));
                        break;
                    case base::android::kLynxArrayType: // LynxArray
                        element = ConvertToLynxArray(env, temp.Get());
                        break;
                    case base::android::kLynxObjectType: // LynxObject
                        element = ConvertToLynxObject(env, temp.Get());
                        break;
                    case base::android::kLynxFunctionObjectType: // LynxFunctionObject
                        element = jscore::LynxValue::MakeFunctionObject(
                                ConvertToLynxFunctionObject(env, temp.Get()));
                        break;
                    case base::android::kVoidType:
                    default:
                        break;
                }
                array->Push(element);
            }
            return array;
        }


        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jintArray java_array) {
            int length = env->GetArrayLength(java_array);
            jint* element = env->GetIntArrayElements(java_array, 0);
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* value = jscore::LynxValue::MakeInt(element[i]);
                array->Push(value);
            }
            return array;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jshortArray java_array) {
            int length = env->GetArrayLength(java_array);
            jshort* element = env->GetShortArrayElements(java_array, 0);
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* value = jscore::LynxValue::MakeInt(element[i]);
                array->Push(value);
            }
            return array;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jlongArray java_array) {
            int length = env->GetArrayLength(java_array);
            jlong* element = env->GetLongArrayElements(java_array, 0);
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* value = jscore::LynxValue::MakeInt(element[i]);
                array->Push(value);
            }
            return array;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jfloatArray java_array) {
            int length = env->GetArrayLength(java_array);
            jfloat* element = env->GetFloatArrayElements(java_array, 0);
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* value = jscore::LynxValue::MakeDouble(element[i]);
                array->Push(value);
            }
            return array;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jdoubleArray java_array) {
            int length = env->GetArrayLength(java_array);
            jdouble* element = env->GetDoubleArrayElements(java_array, 0);
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* value = jscore::LynxValue::MakeDouble(element[i]);
                array->Push(value);
            }
            return array;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jbooleanArray java_array) {
            int length = env->GetArrayLength(java_array);
            jboolean* element = env->GetBooleanArrayElements(java_array, 0);
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* value = jscore::LynxValue::MakeBool(element[i]);
                array->Push(value);
            }
            return array;
        }

        jscore::LynxArray* JNIHelper::ConvertToLynxArray(JNIEnv *env, jbyteArray java_array) {
            int length = env->GetArrayLength(java_array);
            jbyte* element = env->GetByteArrayElements(java_array, 0);
            jscore::LynxArray* array = lynx_new jscore::LynxArray();
            for (int i = 0; i < length; ++i) {
                jscore::LynxValue* value = jscore::LynxValue::MakeInt(element[i]);
                array->Push(value);
            }
            return array;
        }

    }
}