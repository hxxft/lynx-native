#ifndef LYNX_GL_NATIVE_WIDGET_TYPES_H_
#define LYNX_GL_NATIVE_WIDGET_TYPES_H_

#ifndef OS_ANDROID 
#define OS_ANDROID
#endif

#if defined(OS_ANDROID)
#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#endif

namespace gl {
  #if defined(OS_ANDROID)
  typedef ANativeWindow* AcceleratedWidget;
  typedef struct ANativeWindow* EGLNativeWindowType;
  const AcceleratedWidget kNullAcceleratedWidget = 0;
  #else
    #error unknow platform
  #endif

}

#endif