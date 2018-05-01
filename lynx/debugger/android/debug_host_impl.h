// Copyright 2017 The Lynx Authors. All rights reserved.

#include "debugger/debug_host.h"

#include "base/android/android_jni.h"

namespace debug {
class DebugHostImpl : public DebugHost {
 public:
  DebugHostImpl(jobject obj);
  ~DebugHostImpl();
  virtual void Run(DebugType type) override;
  static bool RegisterJNIUtils(JNIEnv* env);

 private:
  base::android::ScopedGlobalJavaRef<jobject> bridge_;
};
}  // namespace debug