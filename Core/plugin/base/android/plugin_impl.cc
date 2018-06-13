#include "plugin/base/android/plugin_impl.h"

#include "PluginImpl_jni.h"

void ReturnBack(JNIEnv* env,
                jobject jcaller,
                jlong nativePtr,
                jlong clientId,
                jlong methodId,
                jboolean successed,
                jobjectArray args) {
  plugin::PluginImpl* impl = reinterpret_cast<plugin::PluginImpl*>(nativePtr);
  if (impl != NULL) {
    impl->Return(clientId, methodId, successed,
                 base::android::JNIHelper::ConvertToLynxArray(env, args));
  }
}

void DispatchEvent(JNIEnv* env,
                   jobject jcaller,
                   jlong nativePtr,
                   jstring event,
                   jobjectArray args) {
  plugin::PluginImpl* impl = reinterpret_cast<plugin::PluginImpl*>(nativePtr);
  if (impl != NULL) {
    impl->DispatchEvent(
        impl->name(),
        base::android::JNIHelper::ConvertToString(env, event),
        base::android::JNIHelper::ConvertToLynxArray(env, args));
  }
}

namespace plugin {
PluginImpl::PluginImpl() {}
PluginImpl::PluginImpl(const std::string& name) : Plugin(), name_(name){
  JNIEnv* env = base::android::AttachCurrentThread();
  plugin_impl_.Reset(
      nullptr,
      Java_PluginImpl_create(
          env,
          (jstring)base::android::LxJType::NewString(env, name.c_str()).Get(),
          reinterpret_cast<long>(this)));
}

PluginImpl::~PluginImpl() {}

void PluginImpl::Exec(long client_id, base::ScopedPtr<jscore::LynxArray> args) {
  if (!plugin_impl_.IsNull()) {
    JNIEnv* env = base::android::AttachCurrentThread();
    Java_PluginImpl_exec(
        base::android::AttachCurrentThread(), plugin_impl_.Get(), client_id,
        (jobjectArray)(base::android::JNIHelper::ConvertToJNIObjectArray(env, args.Get()).Get()));
  }
}

void PluginImpl::AddEvent(const std::string& event) {
  if (!plugin_impl_.IsNull()) {
    JNIEnv* env = base::android::AttachCurrentThread();
    Java_PluginImpl_addEvent(
        base::android::AttachCurrentThread(), plugin_impl_.Get(),
        (jstring)base::android::LxJType::NewString(env, event.c_str()).Get());
  }
}

void PluginImpl::RemoveEvent(const std::string& event) {
  if (!plugin_impl_.IsNull()) {
    JNIEnv* env = base::android::AttachCurrentThread();
    Java_PluginImpl_removeEvent(
        base::android::AttachCurrentThread(), plugin_impl_.Get(),
        (jstring)base::android::LxJType::NewString(env, event.c_str()).Get());
  }
}

bool PluginImpl::RegisterJNIUtils(JNIEnv* env) {
  return RegisterNativesImpl(env);
}

Plugin* Plugin::Create(const std::string& name) {
  return new PluginImpl(name);
}
}  // namespace plugin