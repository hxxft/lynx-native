// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_NET_URL_REQUEST_ANDROID_H_
#define LYNX_NET_URL_REQUEST_ANDROID_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "base/android/android_jni.h"
#include "net/url_request.h"
#include "net/url_request_delegate.h"

namespace net {
    class URLRequestAndroid : public URLRequest {
    public:
        URLRequestAndroid(URLRequestContext *context,
                          const std::string& url,
                          URLRequestDelegate* delegate);

        virtual ~URLRequestAndroid();

        // ThreadSafe
        virtual void Fetch();

        // NotThreadSafe
        // Should be called on UI thread.
        virtual void Cancel();
        static bool RegisterJNIUtils(JNIEnv* env);
    private:
        void CreateRequestObject(const std::string& url);
        base::android::ScopedGlobalJavaRef<jobject> net_request_obj_;
    };
}
#endif  // LYNX_NET_URL_REQUEST_ANDROID_H_
