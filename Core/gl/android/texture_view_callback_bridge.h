#ifndef LYNX_GL_ANDROID_TEXTURE_VIEW_CALLBACK_BRIDGE_H_
#define LYNX_GL_ANDROID_TEXTURE_VIEW_CALLBACK_BRIDGE_H_

#include <jni.h>

namespace gl {
class TextureViewCallbackBridge {
 public:
  static bool RegisterJNIUtils(JNIEnv* env);
};
}  // namespace gl

#endif