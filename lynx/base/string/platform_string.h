// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef BASE_PLATFORM_STRING_H_
#define BASE_PLATFORM_STRING_H_

#include <string>

#if OS_ANDROID
#include <jni.h>
#include "base/android/android_jni.h"
#include "base/android/scoped_java_ref.h"
#else
#import <Foundation/Foundation.h>
#endif

namespace base {
#if OS_ANDROID
    class PlatformString {
    public:
        PlatformString(JNIEnv* env, jstring jstr) : str_(env, jstr) {

        }

        ~PlatformString() {

        }

        virtual const char* GetUTFChars() {
            JNIEnv* env = base::android::AttachCurrentThread();
            if(env) {
                return env->GetStringUTFChars((jstring)str_.Get(), NULL);
            }
            return NULL;
        }

        std::string ToString() {
            std::string str(GetUTFChars());
            return str;
        }

        std::string ToString() const {
            JNIEnv* env = base::android::AttachCurrentThread();
            if(env && str_.Get() != NULL) {
                return std::string(env->GetStringUTFChars((jstring)str_.Get(), NULL));
            }
            return "";
        }
    private:
        android::ScopedGlobalJavaRef<jstring> str_;
    };
#else
    class PlatformString {
    public:
        PlatformString(NSString* str) : string_(str) {}
        ~PlatformString() { string_ = nil; }
        virtual NSString* GetString() const { return string_; }
        
        virtual const char* GetUTFChars() const {
            return [string_ UTF8String];
        }
        
        std::string ToString() const {
            std::string str([string_ UTF8String]);
            return str;
        }
    private:
        NSString* string_;
    };
#endif
}

#endif
