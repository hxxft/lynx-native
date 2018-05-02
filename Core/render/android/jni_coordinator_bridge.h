// Copyright 2017 The Lynx Authors. All rights reserved.

#if OS_ANDROID
#ifndef LYNX_RENDER_COORDINATOR_BRIDGE_H_
#define LYNX_RENDER_COORDINATOR_BRIDGE_H_


#include <string>
#include <jni.h>

#include "base/android/android_jni.h"

namespace lynx {

class JNICoordinatorBridge {
 public:

    static bool RegisterJNIUtils(JNIEnv* env);

};

}  // namespace jscore

#endif  // LYNX_RENDER_COORDINATOR_BRIDGE_H_
#endif
