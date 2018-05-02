// Copyright 2017 The Lynx Authors. All rights reserved.


#ifndef LYNX_TEST_GTEST_DRIVER_H_
#define LYNX_TEST_GTEST_DRIVER_H_


#include <string>
#include <jni.h>

#include "base/android/android_jni.h"

namespace test {

class GTestBridge {
 public:
    static bool RegisterJNIUtils(JNIEnv* env);
};

}  // namespace test

#endif  // LYNX_TEST_GTEST_DRIVER_H_
