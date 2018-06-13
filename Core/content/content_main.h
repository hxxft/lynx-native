#ifndef LYNX_CORE_CONTENT_CONTENT_MAIN_H_
#define LYNX_CORE_CONTENT_CONTENT_MAIN_H_

#if OS_ANDROID
#include "jni.h"
#endif

namespace content {
class ContentMain {
 public:
  static void Initialize();

  #if OS_ANDROID
  static bool RegisterContentMain(JNIEnv* env);
  #endif
};
}  // namespace content

#endif