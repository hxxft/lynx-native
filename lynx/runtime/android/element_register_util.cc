// Copyright 2017 The Lynx Authors. All rights reserved.
#include "base/android/android_jni.h"
#include "RegisterUtil_jni.h"
#include "runtime/element.h"
#include "base/android/jni_helper.h"
#include "runtime/android/element_register_util.h"

static void RegisterTag(JNIEnv* env, jclass jcaller,
                        jint type,
                        jstring tagName){
    auto tag_name = base::android::JNIHelper::ConvertToString(env, tagName);

    jscore::Element::s_element_tags[tag_name] = type;
}

void RegisterJSMethod(JNIEnv* env, jclass jcaller,
                      jint type,
                      jstring methodName,
                      jint methodId) {
    auto method_name = base::android::JNIHelper::ConvertToString(env, methodName);

    jscore::Element::s_rpc_methods[type]
            .insert(std::pair<std::string ,int >(method_name,methodId));
}

namespace lynx {
        bool ElementRegisterUtil::RegisterJNIUtils(JNIEnv* env) {
            return RegisterNativesImpl(env);
        }
}