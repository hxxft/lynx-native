// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/android/java_type.h"
#include "net/url_request_context.h"
#include "net/android/url_request_android.h"
#include "net/url_request_ghost_delegate.h"
#include "runtime/js_context.h"
#include "URLRequest_jni.h"

// This method will be called on UI thread
void OnSuccess(JNIEnv* env, jobject jcaller, jlong ptr, jstring url, jstring response) {
    net::URLRequestGhostDelegate* ghost_delegate
            = reinterpret_cast<net::URLRequestGhostDelegate*>(ptr);
    if (ghost_delegate == NULL) return;
    base::PlatformString scoped_url(env, url);
    base::PlatformString scoped_response(env, response);
    ghost_delegate->DeliverSuccess(scoped_url, scoped_response);
}

// This method will be called on UI thread
void OnFailed(JNIEnv* env, jobject jcaller, jlong ptr, jstring url, jstring error) {
    net::URLRequestGhostDelegate* ghost_delegate
            = reinterpret_cast<net::URLRequestGhostDelegate*>(ptr);
    if (ghost_delegate == NULL) return;
    base::PlatformString scoped_url(env, url);
    base::PlatformString scoped_error(env, error);
    ghost_delegate->DeliverError(scoped_url, scoped_error);
}

namespace net {

    URLRequestAndroid::URLRequestAndroid(URLRequestContext* context,
                                         const std::string& url,
                                         URLRequestDelegate* delegate)
            : URLRequest(context, url, delegate) {
        CreateRequestObject(url);
    }

    URLRequestAndroid::~URLRequestAndroid() {

    }

    bool URLRequestAndroid::RegisterJNIUtils(JNIEnv* env) {
        return RegisterNativesImpl(env);
    }

    void URLRequestAndroid::Fetch() {
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_URLRequest_fetch(env,
                              net_request_obj_.Get());
    }

    void URLRequestAndroid::Cancel() {
        URLRequest::Cancel();
        JNIEnv* env = base::android::AttachCurrentThread();
        Java_URLRequest_cancel(env,
                              net_request_obj_.Get());
    }

    void URLRequestAndroid::CreateRequestObject(const std::string& url) {
        JNIEnv* env = base::android::AttachCurrentThread();
        base::android::ScopedLocalJavaRef<jstring> url_java
                = base::android::JType::NewString(env, url.c_str());
        base::android::ScopedLocalJavaRef<jobject> request_obj
                = Java_URLRequest_create(env,
                                         (jstring) url_java.Get(),
                                         reinterpret_cast<long>(ghost_delegate_));
        net_request_obj_.Reset(env, request_obj);
    }

}

