// Copyright 2017 The Lynx Authors. All rights reserved.

#include "debugger/android/debug_host_impl.h"
#include "DebugHostImpl_jni.h"
#include "debugger/debug_client.h"

void Attach(JNIEnv* env, jobject jcaller) {
  debug::DebugClient::Debugger()->Initialize(
      lynx_new debug::DebugHostImpl(jcaller));
}

namespace debug {

DebugHostImpl::DebugHostImpl(jobject obj) {
  JNIEnv* env = base::android::AttachCurrentThread();
  bridge_.Reset(env, obj);
}

DebugHostImpl::~DebugHostImpl() {}

void DebugHostImpl::Run(DebugType type) {
  if (!bridge_.IsNull()) {
    JNIEnv* env = base::android::AttachCurrentThread();
    Java_DebugHostImpl_runDebug(env, bridge_.Get(), type);
  }
}

bool DebugHostImpl::RegisterJNIUtils(JNIEnv* env) {
  return RegisterNativesImpl(env);
}
}  // namespace debug