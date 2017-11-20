// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/android/jni_coordinator_bridge.h"
#include "CommandExecutor_jni.h"

#include "base/android/jni_helper.h"
#include "base/android/android_jni.h"
#include "render/coordinator/coordinator_executor.h"

jdoubleArray GenerateAction(JNIEnv *env, lynx::CoordinatorAction action) {
    int length = 15;
    jdoubleArray result = env->NewDoubleArray(length);
    double temp[length];
    temp[0] = action.translate_x_;
    temp[1] = action.translate_y_;
    temp[2] = action.scale_x_;
    temp[3] = action.scale_y_;
    temp[4] = action.rotate_x_;
    temp[5] = action.rotate_y_;
    temp[6] = action.pivot_x_;
    temp[7] = action.pivot_y_;
    temp[8] = action.alpha_;
    temp[9] = action.offset_top_;
    temp[10] = action.offset_left_;
    temp[11] = action.offset_bottom_;
    temp[12] = action.offset_right_;
    temp[13] = action.consumed_;
    temp[14] = action.duration_;
    env->SetDoubleArrayRegion(result, 0, length, temp);
    return result;
}

jobject GenerateEvent(JNIEnv *env, lynx::CoordinatorAction action) {
    if (action.event_.empty())
        return NULL;
    int length = 2;
    auto result = base::android::JType::NewObjectArray(env, length);
    auto event_name = base::android::JNIHelper::ConvertToJNIString(env, action.event_);
    env->SetObjectArrayElement(result.Get(), 0, event_name.Get());
    if (action.params_for_event_ != NULL) {
        switch (action.params_for_event_.type_) {
            case lepus::ValueType::Value_String: {
                const char* temp = action.params_for_event_.str_->c_str();
                auto params = base::android::JNIHelper::ConvertToJNIString(env, (char *) temp);
                env->SetObjectArrayElement(result.Get(), 1, params.Get());
            }
                break;
            case lepus::ValueType::Value_Number:{
                double temp = action.params_for_event_.number_;
                auto params = base::android::JType::NewDouble(env, temp);
                env->SetObjectArrayElement(result.Get(), 1, params.Get());
            }
                break;
            case lepus::ValueType::Value_Boolean:{
                bool temp = action.params_for_event_.boolean_;
                auto params = base::android::JType::NewBoolean(env, temp);
                env->SetObjectArrayElement(result.Get(), 1, params.Get());
            }
                break;
            default: break;
        }
    }
    return result.Release();
}

jobjectArray ConvertCoordinatorActionForAndroid(JNIEnv *env, lynx::CoordinatorAction action) {
    int length = 2;
    auto scoped_array = base::android::JType::NewObjectArray(env, length);
    jobjectArray result = scoped_array.Get();
    scoped_array.Release();
    env->SetObjectArrayElement(result, 0, GenerateAction(env, action));
    env->SetObjectArrayElement(result, 1, GenerateEvent(env, action));
    return result;
}

const std::vector<lepus::Value> ConstructCoordinatorArgs(JNIEnv *env, jint tag, jdoubleArray args) {
    int length = env->GetArrayLength(args);
    double *c_args = env->GetDoubleArrayElements(args, JNI_FALSE);
    std::vector<lepus::Value> lepus_args;
    lepus_args.push_back(tag);
    for (int i = 0; i < length; ++i) {
        lepus_args.push_back(c_args[i]);
    }
    return lepus_args;
}

jobjectArray Execute(JNIEnv* env, jclass jcaller, jlong ptr,
                          jstring method, jint tag, jdoubleArray args) {
    lynx::CoordinatorExecutor* executor = reinterpret_cast<lynx::CoordinatorExecutor *>(ptr);
    std::string method_str = base::android::JNIHelper::ConvertToString(env, method);
    lynx::CoordinatorAction action = executor->Execute(method_str,
                                                        ConstructCoordinatorArgs(env, tag, args));
    return ConvertCoordinatorActionForAndroid(env, action);
}

jlong Prepare(JNIEnv *env, jclass jcaller, jstring executable) {
    std::string content = base::android::JNIHelper::ConvertToString(env, executable);
    lynx::CoordinatorExecutor *executor = lynx_new lynx::CoordinatorExecutor(content);
    return reinterpret_cast<jlong>(executor);
}

void Destroy(JNIEnv *env, jclass jcaller, jlong ptr) {
}

namespace lynx {

    bool JNICoordinatorBridge::RegisterJNIUtils(JNIEnv* env) {
        return RegisterNativesImpl(env);
    }

}