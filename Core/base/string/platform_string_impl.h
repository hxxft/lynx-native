// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef BASE_PLATFORM_STRING_IMPL_H_

#include "base/string/platform_string.h"

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
    class PlatformStringImpl : public PlatformString {
    public:
        PlatformStringImpl(JNIEnv* env, jstring jstr) : str_(env, jstr) {
            
        }
        
        virtual ~PlatformStringImpl() {
            
        }
        
        virtual const char* GetUTFChars() const {
            JNIEnv* env = base::android::AttachCurrentThread();
            if(env) {
                return env->GetStringUTFChars((jstring)str_.Get(), NULL);
            }
            return NULL;
        }
        
//        std::string ToString() {
//            std::string str(GetUTFChars());
//            return str;
//        }
        
        virtual std::string ToString() const {
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
    class PlatformStringImpl : public PlatformString {
    public:
        PlatformStringImpl(NSString* str) : string_(str) {}
        virtual ~PlatformStringImpl() { string_ = nil; }
        
        virtual const char* GetUTFChars() const {
            return [string_ UTF8String];
        }
        
        virtual std::string ToString() const {
            std::string str([string_ UTF8String]);
            return str;
        }
        
         NSString* GetNSString() const { return string_; }
    private:
        NSString* string_;
    };
#endif
}

#endif  // BASE_PLATFORM_STRING_IMPL_H_
