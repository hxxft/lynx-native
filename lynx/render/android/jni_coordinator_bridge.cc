// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/android/jni_coordinator_bridge.h"
#include "CrdCommandExecutor_jni.h"

#include "base/android/jni_helper.h"
#include "render/coordinator/coordinator_executor.h"

base::android::ScopedLocalJavaRef<jdoubleArray> GenerateAction(JNIEnv *env,
                                                               lynx::CoordinatorAction action) {
    int length = 16;
    auto result = base::android::JType::NewDoubleArray(env, length);
    double temp[length];
    temp[0] = action.translate_x_;
    temp[1] = action.translate_y_;
    temp[2] = action.scale_x_;
    temp[3] = action.scale_y_;
    temp[4] = action.rotate_x_;
    temp[5] = action.rotate_y_;
    temp[6] = action.origin_x_;
    temp[7] = action.origin_y_;
    temp[8] = action.opacity_;
    temp[9] = action.top_offset_;
    temp[10] = action.left_offset_;
    temp[11] = action.bottom_offset_;
    temp[12] = action.right_offset_;
    temp[13] = action.consumed_;
    temp[14] = action.duration_;
    temp[15] = action.timing_function_;
    env->SetDoubleArrayRegion(result.Get(), 0, length, temp);
    return result;
}

base::android::ScopedLocalJavaRef<jobjectArray> GenerateEvent(JNIEnv *env,
                                                         lynx::CoordinatorAction action) {
    if (action.event_.empty())
        return base::android::ScopedLocalJavaRef<jobjectArray>();
    int length = 2;
    auto result = base::android::LxJType::NewObjectArray(env, length);
    auto event_name = base::android::JNIHelper::ConvertToJNIString(env, action.event_);
    env->SetObjectArrayElement(result.Get(), 0, event_name.Get());
    switch (action.params_for_event_.type_) {
        case lepus::ValueType::Value_String: {
            const char* temp = action.params_for_event_.str_->c_str();
            auto params = base::android::JNIHelper::ConvertToJNIString(env, (char *) temp);
            env->SetObjectArrayElement(result.Get(), 1, params.Get());
        }
            break;
        case lepus::ValueType::Value_Number:{
            double temp = action.params_for_event_.number_;
            auto params = base::android::LxJType::NewDouble(env, temp);
            env->SetObjectArrayElement(result.Get(), 1, params.Get());
        }
            break;
        case lepus::ValueType::Value_Boolean:{
            bool temp = action.params_for_event_.boolean_;
            auto params = base::android::LxJType::NewBoolean(env, temp);
            env->SetObjectArrayElement(result.Get(), 1, params.Get());
        }
            break;
        default: break;
    }
    return result;
}

base::android::ScopedLocalJavaRef<jobjectArray>
ConvertCoordinatorActionForAndroid(JNIEnv *env, lynx::CoordinatorAction action) {
    int length = 2;
    auto result = base::android::LxJType::NewObjectArray(env, length);
    env->SetObjectArrayElement(result.Get(), 0, GenerateAction(env, action).Get());
    env->SetObjectArrayElement(result.Get(), 1, GenerateEvent(env, action).Get());
    return result;
}

const std::vector<lepus::Value> ConstructCoordinatorArgs(JNIEnv *env,
                                                         lynx::CoordinatorExecutor* executor,
                                                         jstring tag,
                                                         jdoubleArray args) {
    int length = env->GetArrayLength(args);
    double *c_args = env->GetDoubleArrayElements(args, JNI_FALSE);
    std::vector<lepus::Value> lepus_args;

    lepus::Value lepus_tag;
    lepus_tag.type_ = lepus::Value_String;
    if (tag != NULL) {
        lepus_tag.str_ = executor->context()
                ->string_pool()
                ->NewString(base::android::JNIHelper::ConvertToString(env, tag).c_str());
    } else {
        lepus_tag.str_ = lepus_tag.str_ = executor->context()
                ->string_pool()
                ->NewString("");
    }
    lepus_tag.str_->AddRef();

    lepus_args.push_back(lepus_tag);
    for (int i = 0; i < length; ++i) {
        lepus_args.push_back(c_args[i]);
    }

    env->ReleaseDoubleArrayElements(args, c_args, JNI_FALSE);

    return lepus_args;
}

jobjectArray Execute(JNIEnv* env, jclass jcaller, jlong ptr,
                          jstring method, jstring tag, jdoubleArray args) {
    lynx::CoordinatorExecutor* executor = reinterpret_cast<lynx::CoordinatorExecutor *>(ptr);
    std::string method_str = base::android::JNIHelper::ConvertToString(env, method);
    lynx::CoordinatorAction action = executor->Execute(method_str,
                                                        ConstructCoordinatorArgs(env,
                                                                                 executor,
                                                                                 tag,
                                                                                 args));
    return ConvertCoordinatorActionForAndroid(env, action).Release();
}

jlong Prepare(JNIEnv *env, jclass jcaller, jstring executable) {
    std::string content = base::android::JNIHelper::ConvertToString(env, executable);
    lynx::CoordinatorExecutor *executor = lynx_new lynx::CoordinatorExecutor(content);
    return reinterpret_cast<jlong>(executor);
}

void Destroy(JNIEnv *env, jclass jcaller, jlong ptr) {
}

jboolean UpdateProperty(JNIEnv *env,
                    jclass jcaller,
                    jlong ptr,
                    jstring property,
                    jint type,
                    jstring value1,
                    jdouble value2,
                    jboolean value3) {
    if (property == NULL)
        return JNI_FALSE;

    lynx::CoordinatorExecutor *executor = reinterpret_cast<lynx::CoordinatorExecutor *>(ptr);
    std::string name = base::android::JNIHelper::ConvertToString(env, property);
    lepus::Value lepus_value;
    // Add string
    if (type == 0) {
        lepus_value.type_ = lepus::Value_String;
        if (value1 != NULL) {
            lepus_value.str_ = executor->context()->string_pool()
                    ->NewString(base::android::JNIHelper::ConvertToString(env, value1).c_str());
        } else {
            lepus_value.str_ = lepus_value.str_ = executor->context()->string_pool()->NewString("");
        }
        lepus_value.str_->AddRef();
    }
    // Add double
    else if (type == 1) {
        lepus_value = value2;
    }
    // Add boolean
    else if (type == 2) {
        lepus_value.type_ = lepus::Value_Boolean;
        lepus_value.boolean_ = value3;
    }
    if (executor->context()->UpdateTopLevelVariable(name, lepus_value)) {
        return JNI_TRUE;
    }

    return JNI_FALSE;

}

namespace lynx {

    bool JNICoordinatorBridge::RegisterJNIUtils(JNIEnv* env) {
        return RegisterNativesImpl(env);
    }

}