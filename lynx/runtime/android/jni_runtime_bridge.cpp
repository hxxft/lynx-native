// Copyright 2017 The Lynx Authors. All rights reserved.



#include "runtime/android/jni_runtime_bridge.h"

#include "base/debug/memory_debug.h"
#include "base/android/jni_helper.h"
#include "config/global_config_data.h"
#include "render/android/render_tree_host_impl_android.h"
#include "render/android/render_object_impl_android.h"

#include "base/print.h"

#include "LynxRuntime_jni.h"

#if USING_V8
#include "runtime/v8/v8_context.h"
#else
#include "runtime/jsc/jsc_context.h"
#endif


jint CreateNativeJSRuntime(JNIEnv* env, jobject jcaller) {
    jscore::Runtime* runtime_ptr = 
#if USING_V8
        lynx_new jscore::Runtime(lynx_new jscore::V8Context());
#else
        lynx_new jscore::Runtime(lynx_new jscore::JSCContext());
#endif
    return reinterpret_cast<long>(runtime_ptr);
}

void DestroyNativeJSRuntime(JNIEnv* env, jobject jcaller, jlong runtime) {
    LOGD("lynx-debug", "DestroyNativeJSRuntime");
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    if(runtime_ptr == NULL) return ;

    runtime_ptr->Destroy();
}

void RunScript(JNIEnv *env, jobject jcaller, jlong runtime, jstring source) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    LOGD("lynx-debug", "RunScript %p", runtime_ptr);
    base::PlatformString platform_string(env, source);
    runtime_ptr->RunScript(platform_string);
}

void LoadHTML(JNIEnv *env, jobject jcaller, jlong runtime, jstring url, jstring source) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    LOGD("lynx-debug", "RunScript %p", runtime_ptr);
    base::PlatformString platform_string(env, source);
    base::PlatformString platform_url(env, url);
    runtime_ptr->LoadHTML(platform_url.ToString(), platform_string.ToString());
}

void LoadUrl(JNIEnv *env, jobject jcaller, jlong runtime, jstring url) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    runtime_ptr->LoadUrl(base::android::JNIHelper::ConvertToString(env, url));
}

void InitRuntime(JNIEnv *env, jobject caller, jlong runtime) {

    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    LOGD("lynx-debug", "InitRuntime %p", runtime_ptr);
    if (runtime_ptr == NULL) return;
    runtime_ptr->SetupRenderHost();
    runtime_ptr->InitRuntime("");
}

jobject ActiveRuntime(JNIEnv *env, jobject caller, jlong runtime, jint width, jint height, jdouble density) {

    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    LOGD("lynx-debug", "ActiveRuntime %p", runtime_ptr);
    if (runtime_ptr == NULL) return 0;

    if (!config::GlobalConfigData::GetInstance()->is_initialized()) {
        config::GlobalConfigData::GetInstance()->SetScreenConfig(
            width, height, density);
    }

    return reinterpret_cast<lynx::RenderTreeHostImplAndroid*>(
               runtime_ptr->render_tree_host()->host_impl())->GetJavaImpl();
}

jstring GetPageURL(JNIEnv* env, jobject jcaller, jlong runtime) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    if (runtime_ptr == NULL) return 0;
    return (jstring) base::android::JNIHelper::ConvertToJNIString(env,
                                                                  runtime_ptr->GetPageUrl())
            .Get();
}

jstring GetUserAgent(JNIEnv* env, jobject jcaller, jlong runtime) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    if (runtime_ptr == NULL) return 0;
    return (jstring) base::android::JNIHelper::ConvertToJNIString(env,
                                                                  runtime_ptr->GetUserAgent())
            .Get();
}

void AddJavascriptInterface(JNIEnv* env,
                            jobject jcaller,
                            jlong runtime,
                            jobject object,
                            jstring name) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    if (runtime_ptr == NULL) return;
    runtime_ptr->AddJavaScriptInterface(base::android::JNIHelper::ConvertToString(env, name),
                    base::android::JNIHelper::ConvertToLynxFunctionObject(env, object).Release());
}

void CheckMemoryLeak(JNIEnv* env, jclass jcaller) {
    base::MemoryTracker::Instance()->PrintMemInfo();
}

jboolean CheckMemoryEnabled(JNIEnv* env, jclass jcaller) {
#if DEBUG_MEMORY
    return true;
#else
    return false;
#endif
}

namespace jscore {

bool JNIRuntimeBridge::RegisterJNIUtils(JNIEnv* env) {
    return RegisterNativesImpl(env);
}

}
