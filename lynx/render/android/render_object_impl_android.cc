// Copyright 2017 The Lynx Authors. All rights reserved.

#include <android/bitmap.h>
#include "render/canvas_view.h"

#include "render/android/render_object_impl_android.h"

#include "base/android/jni_helper.h"
#include "runtime/base/lynx_value.h"
#include "runtime/platform_value.h"

#include "base/android/android_jni.h"
#include "base/android/convert.h"

#include "render/render_tree_host.h"

#include "RenderObjectImpl_jni.h"
#include "render/render_object.h"

void DispatchEvent(JNIEnv* env,
                   jobject jcaller,
                   jlong nativePtr,
                   jstring event,
                   jobjectArray args) {
    lynx::RenderObjectImplAndroid* impl =
        reinterpret_cast<lynx::RenderObjectImplAndroid*>(nativePtr);
    if (impl != NULL) {
        impl->DispatchEvent(env, event, args);
    }
}

void UpdateData(JNIEnv* env,
                  jobject jcaller,
                  jlong native_ptr,
                  jint attr,
                  jobject value) {
    lynx::RenderObjectImplAndroid* impl =
        reinterpret_cast<lynx::RenderObjectImplAndroid*>(native_ptr);
    if (impl != NULL) {
        impl->UpdateData(env, attr, value);
    }
}

void FreeJavaRef(JNIEnv* env, jobject jcaller,
                 jlong nativePtr){
    lynx::RenderObjectImplAndroid* impl =
            reinterpret_cast<lynx::RenderObjectImplAndroid*>(nativePtr);
    if (impl != NULL) {
        impl->FreeJavaRef(env);
    }
}

void ConstructJavaRef(JNIEnv* env, jobject jcaller,
                      jlong nativePtr,
                      jobject object){
    lynx::RenderObjectImplAndroid* impl =
            reinterpret_cast<lynx::RenderObjectImplAndroid*>(nativePtr);
    if (impl != NULL) {
        impl->ConstructJavaRef(env, object);
    }

}

namespace lynx {

bool RenderObjectImplAndroid::RegisterJNIUtils(JNIEnv* env) {
    return RegisterNativesImpl(env);
}

RenderObjectImplAndroid::RenderObjectImplAndroid(
    jscore::ThreadManager* manager,
    RenderObjectType type)
    : RenderObjectImpl(manager, type) {

    JNIEnv* env = base::android::AttachCurrentThread();
    render_object_impl_java_impl_.Reset(nullptr,
                                        Java_RenderObjectImpl_create(
                                            env,
                                            type,
                                            reinterpret_cast<long>(this)));
}

RenderObjectImplAndroid::~RenderObjectImplAndroid() {
}

void RenderObjectImplAndroid::UpdateStyle(const lynx::CSSStyle& style) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_updateStyle(
            env,
            render_object_impl_java_impl_.Get(),
            base::Convert::StyleConvert(style).Get());
    }
}

void RenderObjectImplAndroid::SetPosition(const base::Position& position) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_setPosition(
            env,
            render_object_impl_java_impl_.Get(),
            base::Convert::PositionConvert(position).Get());
    }
}

void RenderObjectImplAndroid::SetSize(const base::Size& size) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_setSize(env,
                                      render_object_impl_java_impl_.Get(),
                                      base::Convert::SizeConvert(size).Get());
    }
}

void RenderObjectImplAndroid::InsertChild(RenderObjectImpl* child, int index) {
    RenderObjectImplAndroid* element_proxy =
        static_cast<RenderObjectImplAndroid*>(child);
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_insertChild(env,
                                          render_object_impl_java_impl_.Get(),
                                          element_proxy->java_impl(), index);
    }
}

void RenderObjectImplAndroid::RemoveChild(RenderObjectImpl* child) {
    RenderObjectImplAndroid* element_proxy =
        static_cast<RenderObjectImplAndroid*>(child);
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_removeChild(
            env,
            render_object_impl_java_impl_.Get(),
            element_proxy->java_impl());
    }
}

void RenderObjectImplAndroid::SetText(const std::string& text) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_setText(
            env,
            render_object_impl_java_impl_.Get(),
            (jstring)base::android::LxJType::NewString(env, text.c_str()).Get());
    }
}

void RenderObjectImplAndroid::SetAttribute(
    const std::string& key,
    const std::string& value) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_setAttribute(
            env,
            render_object_impl_java_impl_.Get(),
            (jstring)base::android::LxJType::NewString(env, key.c_str()).Get(),
            (jstring)base::android::LxJType::NewString(env, value.c_str()).Get());
    }
}

void RenderObjectImplAndroid::RequestLayout() {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_requestLayout(
            env,
            render_object_impl_java_impl_.Get());
    }
}

void RenderObjectImplAndroid::AddEventListener(const std::string& event) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_addEventListener(
            env,
            render_object_impl_java_impl_.Get(),
            (jstring)base::android::LxJType::NewString(env, event.c_str()).Get());
    }
}

void RenderObjectImplAndroid::RemoveEventListener(const std::string& event) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_removeEventListener(
            env,
            render_object_impl_java_impl_.Get(),
            base::android::LxJType::NewString(env, event.c_str()).Get());
    }
}

void RenderObjectImplAndroid::SetData(int key, base::ScopedPtr<jscore::LynxValue> value) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        if (value.Get() != NULL && value->type_ == jscore::LynxValue::Type::VALUE_PLATFORM) {
            Java_RenderObjectImpl_setData(
                    env,
                    render_object_impl_java_impl_.Get(),
                    key,
                    value.Get()->data_.platform_value->value());
        } else {
            base::android::ScopedLocalJavaRef<jobject> jvalue =
                    base::android::JNIHelper::ConvertToJNIObject(env, value.Get());
            Java_RenderObjectImpl_setData(
                    env,
                    render_object_impl_java_impl_.Get(),
                    key,
                    jvalue.Release());
        }
    }
}

void RenderObjectImplAndroid::Animate(base::ScopedPtr<jscore::LynxArray> &keyframes,
                                      base::ScopedPtr<jscore::LynxObject> &options) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        base::android::ScopedLocalJavaRef<jobject> jkeyframes =
                base::android::JNIHelper::ConvertToJNIObject(env, keyframes.Get());
        base::android::ScopedLocalJavaRef<jobject> joptions =
                base::android::JNIHelper::ConvertToJNIObject(env, options.Get());
        Java_RenderObjectImpl_animate(
                env,
                render_object_impl_java_impl_.Get(),
                jkeyframes.Release(),
                joptions.Release());
    }
}


void RenderObjectImplAndroid::CancelAnimation(const std::string &id) {
    if (!render_object_impl_java_impl_.IsNull()) {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_RenderObjectImpl_cancelAnimation(
                env,
                render_object_impl_java_impl_.Get());
    }
}

void RenderObjectImplAndroid::DispatchEvent(
    JNIEnv* env,
    jstring event,
    jobjectArray args) {
    std::string event_str = base::android::JNIHelper::ConvertToString(env, event);
    base::ScopedPtr<jscore::LynxArray> array =
            base::android::JNIHelper::ConvertToLynxArray(env, args);
    if (render_object_weak_ptr_.IsValid()) {
        render_object_weak_ptr_->DispatchEvent(
                event_str,
                array);
    }
}

void RenderObjectImplAndroid::UpdateData(JNIEnv *env,
                                         jint attr,
                                         jobject value) {

    if(attr == lynx::RenderObject::RENDER_OBJECT_ATTRS::CANVAS_IMAGE_DATA){
        native_bitmap_.Reset(env, value);
        return;
    }
    base::ScopedPtr<jscore::LynxValue> value_transformed =
            base::android::JNIHelper::ConvertToLynxValue(env, value);
    if (render_object_weak_ptr_.IsValid()) {
        render_object_weak_ptr_->UpdateData(attr, value_transformed);
    }
}

typedef struct {
    uint8_t alpha;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} argb;

base::ScopedPtr<jscore::LynxObject> RenderObjectImplAndroid::GetImagePixel(int x, int y, int w,int h){
    base::ScopedPtr<jscore::LynxObject> ret(lynx_new jscore::LynxObject());
    JNIEnv* env = base::android::AttachCurrentThread();
    if(native_bitmap_.Get() != NULL){
        AndroidBitmapInfo bmp_info={0};
        if(AndroidBitmap_getInfo(env, native_bitmap_.Get(), &bmp_info)<0) {
            return ret;
        }
        int* pixels_data = NULL;
        if(AndroidBitmap_lockPixels(env, native_bitmap_.Get(),(void**)&pixels_data)){
            return ret;
        }
        int column;
        int row;
        int height = h > (bmp_info.height-y) ? (bmp_info.height-y) : h;
        int width = w > (bmp_info.width-x) ? (bmp_info.width-x) : w;
        ret->Set("height", jscore::LynxValue::MakeInt(height).Release());
        ret->Set("width", jscore::LynxValue::MakeInt(width).Release());
        // 拼接返回对象
        base::ScopedPtr<jscore::LynxArray> pixels(lynx_new jscore::LynxArray());
        argb * line = (argb *) pixels_data;
        for (row = y; row < (y+height); row++) {
            for(column = x; column < (x+width); column++){
                int index = row * bmp_info.width + column;
                pixels->Push(jscore::LynxValue::MakeInt(line[index].red).Release());
                pixels->Push(jscore::LynxValue::MakeInt(line[index].green).Release());
                pixels->Push(jscore::LynxValue::MakeInt(line[index].blue).Release());
                pixels->Push(jscore::LynxValue::MakeInt(line[index].alpha).Release());
            }
        }
        ret->Set("data", pixels.Release());
        AndroidBitmap_unlockPixels(env, native_bitmap_.Get());
    }
    return ret;
}

void RenderObjectImplAndroid::FreeJavaRef(JNIEnv *env) {
    render_object_impl_java_impl_.Reset(env, NULL);
}

void RenderObjectImplAndroid::ConstructJavaRef(JNIEnv *env, jobject object) {
    render_object_impl_java_impl_.Reset(env, object);
}

}  // namespace lynx
