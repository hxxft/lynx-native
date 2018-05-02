// Copyright 2017 The Lynx Authors. All rights reserved.



#include "runtime/android/jni_runtime_bridge.h"

#include "base/android/jni_helper.h"
#include "base/log/logging.h"
#include "base/string/platform_string_impl.h"
#include "config/global_config_data.h"
#include "render/android/render_tree_host_impl_android.h"
#include "render/android/render_object_impl_android.h"
#include "runtime/android/result_callback.h"

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
  DCHECK(runtime);
  jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
  if (runtime_ptr == NULL)
    return;

  runtime_ptr->Destroy();
}

void RunScript(JNIEnv *env, jobject jcaller,
               jlong runtime,
               jstring source,
               jobject callback) {
    jscore::Runtime *runtime_ptr = reinterpret_cast<jscore::Runtime *>(runtime);
    base::ScopedPtr<base::PlatformString> platform_string(new base::PlatformStringImpl(env, source));
    base::ScopedPtr<jscore::ResultCallback> cb;
    if (callback != NULL) {
        cb.Reset(lynx_new jscore::ResultCallbackAndroid(env, callback));
    }
    runtime_ptr->RunScript(platform_string, cb);
}

void LoadHTML(JNIEnv *env, jobject jcaller, jlong runtime, jstring url, jstring source) {
  DCHECK(runtime);
  jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
  base::ScopedPtr<base::PlatformString> platform_string(new base::PlatformStringImpl(env, source));
  base::ScopedPtr<base::PlatformString> platform_url(new base::PlatformStringImpl(env, url));
  runtime_ptr->LoadHTML(platform_url->ToString(), platform_string->ToString());
}

static void LoadScriptDataWithBaseUrl(JNIEnv *env, jobject jcaller,
                                      jlong runtime,
                                      jstring source,
                                      jstring url) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    base::ScopedPtr<base::PlatformString> platform_string(
        new base::PlatformStringImpl(env, source));
    base::ScopedPtr<base::PlatformString> platform_url(
        new base::PlatformStringImpl(env, url));
    runtime_ptr->LoadScriptDataWithBaseUrl(platform_string->ToString(), platform_url->ToString());
}

void LoadUrl(JNIEnv *env, jobject jcaller, jlong runtime, jstring url) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    runtime_ptr->LoadUrl(base::android::JNIHelper::ConvertToString(env, url));
}

jobject InitRuntime(JNIEnv *env, jobject caller, jlong runtime) {
  DCHECK(runtime);
  jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
  if (runtime_ptr == NULL)
    return NULL;
  runtime_ptr->SetupRenderHost();
  runtime_ptr->InitRuntime("");

  return reinterpret_cast<lynx::RenderTreeHostImplAndroid*>(
             runtime_ptr->render_tree_host()->host_impl())
      ->GetJavaImpl();
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
    auto ref = base::android::JNIHelper::ConvertToJNIString(env,
                                                            runtime_ptr->GetUserAgent());
    return (jstring) ref.Release();
}

void SetUserAgent(JNIEnv *env, jobject jcaller,
                     jlong runtime,
                     jstring ua) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    auto str = base::android::JNIHelper::ConvertToString(env, ua);
    runtime_ptr->SetUserAgent(str);
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

void InitGlobalConfig(JNIEnv* env, jclass jcaller,
                      jint screenWidthPx,
                      jint screenHeightPx,
                      jdouble density,
                      jint zoomRatioPx,
                      jstring deviceInfo) {
    std::string device_info = base::android::JNIHelper::ConvertToString(env, deviceInfo);
    config::GlobalConfigData::GetInstance()->SetScreenConfig(
            screenWidthPx, screenHeightPx, density, zoomRatioPx, device_info);
}

void SetExceptionListner(JNIEnv* env, jobject jcaller,
                         jlong runtime,
                         jobject listener) {
    jscore::Runtime* runtime_ptr = reinterpret_cast<jscore::Runtime*>(runtime);
    runtime_ptr->set_exception_handler(lynx_new jscore::ResultCallbackAndroid(env, listener));
}

namespace jscore {

bool JNIRuntimeBridge::RegisterJNIUtils(JNIEnv* env) {
    return RegisterNativesImpl(env);
}

}
