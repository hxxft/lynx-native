// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_ANDROID_RENDER_TREE_HOST_IMPL_ANDROID_H_
#define LYNX_RENDER_ANDROID_RENDER_TREE_HOST_IMPL_ANDROID_H_

#include <jni.h>
#include <base/print.h>

#include "base/android/scoped_java_ref.h"
#include "render/render_tree_host_impl.h"

namespace lynx {
class RenderTreeHostImplAndroid : public RenderTreeHostImpl,
    public RenderTreeHostImpl::VSyncListener {
 public:
    RenderTreeHostImplAndroid(
        jscore::ThreadManager* thread_manager,
        RenderTreeHost* host,
        RenderObjectImpl* root);
    virtual ~RenderTreeHostImplAndroid() {}

    static bool RegisterJNIUtils(JNIEnv* env);

    jobject GetJavaImpl() { return java_render_tree_host_.Get(); }

    virtual void OnVSync();

    virtual void BeginFrame();
    virtual void PrepareCommit();
 private:
    base::android::ScopedGlobalJavaRef<jobject> java_render_tree_host_;
};
}  // namespace lynx

#endif  // LYNX_RENDER_ANDROID_RENDER_TREE_HOST_IMPL_ANDROID_H_
