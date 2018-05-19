#include <jni.h>

#include "base/android/android_jni.h"
#include "gen/LynxTextureViewCallback_jni.h"

#include "gl/android/texture_view_callback_bridge.h"

#include "gl/gl_renderer.h"

jlong OnTextureViewCreate(JNIEnv* env, jclass jcaller) {
    gl::GLRenderer* gl_renderer = new gl::GLRenderer();
    gl_renderer->AddRef();
    gl_renderer->Start();
    return reinterpret_cast<long>(gl_renderer); 
}

void OnSurfaceTextureAvailable(JNIEnv* env,
                               jclass jcaller,
                               jlong renderer,
                               jobject surface,
                               jint width,
                               jint height) {
  base::Size size(width, height);
  gl::GLRenderer* gl_renderer = reinterpret_cast<gl::GLRenderer*>(renderer); 
  ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
  gl_renderer->OnAcceleratedWidgetAvailable(window, size);
}

namespace gl {
bool TextureViewCallbackBridge::RegisterJNIUtils(JNIEnv* env) {
  return RegisterNativesImpl(env);
}
}  // namespace gl
