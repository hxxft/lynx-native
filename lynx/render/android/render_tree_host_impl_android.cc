// Copyright 2017 The Lynx Authors. All rights reserved.

#include "render/android/render_object_impl_android.h"

#include "base/android/android_jni.h"
#include "render/android/render_tree_host_impl_android.h"
#include "LynxRenderTreeHostImpl_jni.h"

void UpdateViewport(JNIEnv* env,
                    jobject jcaller,
                    jlong hostImpl,
                    jint left,
                    jint top,
                    jint right,
                    jint bottom) {
    lynx::RenderTreeHostImplAndroid* host =
        reinterpret_cast<lynx::RenderTreeHostImplAndroid*>(hostImpl);
    if (host != NULL) {
        host->UpdateViewport(left, top, right, bottom);
    }
}

void OnVSync(JNIEnv* env, jobject jcaller, jlong hostImpl) {
    lynx::RenderTreeHostImplAndroid* host =
        reinterpret_cast<lynx::RenderTreeHostImplAndroid*>(hostImpl);
    if (host != NULL) {
        host->OnVSync();
    }
}

namespace lynx {

bool RenderTreeHostImplAndroid::RegisterJNIUtils(JNIEnv* env) {
    return RegisterNativesImpl(env);
}

RenderTreeHostImplAndroid::RenderTreeHostImplAndroid(
    jscore::ThreadManager* thread_manager,
    RenderTreeHost* host,
    RenderObjectImpl* root)
    : RenderTreeHostImpl(thread_manager, host, root) {
    JNIEnv* env = base::android::AttachCurrentThread();
    java_render_tree_host_.Reset(
        nullptr,
        Java_LynxRenderTreeHostImpl_create(
            env,
            reinterpret_cast<long>(this),
            (static_cast<RenderObjectImplAndroid*>(root)->java_impl())));
}

void RenderTreeHostImplAndroid::OnVSync() {
    DoRenderAction();
}

void RenderTreeHostImplAndroid::BeginFrame() {
    Java_LynxRenderTreeHostImpl_beignFrame(
        base::android::AttachCurrentThread(),
        java_render_tree_host_.Get());
    RenderTreeHostImpl::BeginFrame();
}

void RenderTreeHostImplAndroid::PrepareCommit() {
    Java_LynxRenderTreeHostImpl_prepareCommit(
        base::android::AttachCurrentThread(),
        java_render_tree_host_.Get());
}

}  // namespace lynx

