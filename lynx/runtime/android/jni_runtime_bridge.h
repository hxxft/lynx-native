// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_V8_RUNTIME_H_
#define LYNX_RUNTIME_V8_V8_RUNTIME_H_


#include <string>
#include <jni.h>

#include "runtime/runtime.h"

namespace jscore {

class JNIRuntimeBridge {
 public:

    static bool RegisterJNIUtils(JNIEnv* env);

};

}  // namespace jscore

#endif  // LYNX_RUNTIME_V8_V8_RUNTIME_H_
