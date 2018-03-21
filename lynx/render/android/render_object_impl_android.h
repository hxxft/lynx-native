// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RENDER_ANDROID_RENDER_OBJECT_IMPL_ANDROID_H_
#define LYNX_RENDER_ANDROID_RENDER_OBJECT_IMPL_ANDROID_H_

#include <jni.h>
#include <string>

#include "base/android/scoped_java_ref.h"
#include "render/impl/render_object_impl.h"
#include "runtime/thread_manager.h"
#include "runtime/base/lynx_array.h"

namespace lynx {
class RenderObjectImplAndroid : public RenderObjectImpl {
 public:
    RenderObjectImplAndroid(
        jscore::ThreadManager* manager,
        RenderObjectType type);

    virtual ~RenderObjectImplAndroid();

    jobject java_impl() { return render_object_impl_java_impl_.Get(); }

    void DispatchEvent(JNIEnv* env, jstring event, jobjectArray args);
    static bool RegisterJNIUtils(JNIEnv* env);

    void UpdateData(JNIEnv *env, jint attr, jobject value);
    void FreeJavaRef(JNIEnv *env);
    void ConstructJavaRef(JNIEnv *env, jobject object);
    base::ScopedPtr<jscore::LynxObject> GetImagePixel(int x, int y, int w,int h);

    base::android::ScopedGlobalJavaRef<jobject> native_bitmap_;

 private:
    virtual void UpdateStyle(const lynx::CSSStyle& style);
    virtual void SetPosition(const base::Position& position);
    virtual void SetSize(const base::Size& size);
    virtual void InsertChild(RenderObjectImpl* child, int index);
    virtual void RemoveChild(RenderObjectImpl* child);
    virtual void SetText(const std::string& text);
    virtual void SetAttribute(const std::string& key, const std::string& value);
    virtual void RequestLayout();
    virtual void AddEventListener(const std::string& event);
    virtual void RemoveEventListener(const std::string& event);
    virtual void SetData(int key, base::ScopedPtr<jscore::LynxValue> value);
    virtual void Animate(base::ScopedPtr<jscore::LynxArray> &keyframes,
                         base::ScopedPtr<jscore::LynxObject> &options);
    virtual void CancelAnimation(const std::string &id);

 private:
    base::android::ScopedGlobalJavaRef<jobject> render_object_impl_java_impl_;
};
}  // namespace lynx


#endif  // LYNX_RENDER_ANDROID_RENDER_OBJECT_IMPL_ANDROID_H_
