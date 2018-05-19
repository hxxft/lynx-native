// Copyright 2017 The Lynx Authors. All rights reserved.

#include "gl/gl_surface_egl.h"

#include "gl/egl_util.h"

namespace gl {
  EGLDisplay g_display;

  GLSurface::GLSurface() {}

  GLSurface::~GLSurface(){}

  void* GLSurface::GetConfig() {
    return nullptr;
  }

  void* GLSurface::GetDisplay() {
    return nullptr;
  }

  GLSurfaceEGL::GLSurfaceEGL() {}

  GLSurfaceEGL::~GLSurfaceEGL() {}

  EGLDisplay GLSurfaceEGL::GetDisplay() {
    return g_display;
  }

  EGLConfig GLSurfaceEGL::GetConfig() {
    return config_;
  }

  EGLSurface NativeViewGLSurfaceEGL::GetHandle() {
    return surface_;
  }

  NativeViewGLSurfaceEGL::NativeViewGLSurfaceEGL(EGLNativeWindowType window) :
  window_(window){
    #if defined(OS_ANDROID)
    if(window)
      ANativeWindow_acquire(window);
    #endif
  }

  NativeViewGLSurfaceEGL::~NativeViewGLSurfaceEGL(){}

  bool NativeViewGLSurfaceEGL::Initialize() {
    EGLint renderable_type = EGL_OPENGL_ES2_BIT;
    EGLint buffer_size = 32;
    EGLint alpha_size = 8;
    EGLint config_attribs_8888[] = {EGL_BUFFER_SIZE,
                                    buffer_size,
                                    EGL_ALPHA_SIZE,
                                    alpha_size,
                                    EGL_BLUE_SIZE,
                                    8,
                                    EGL_GREEN_SIZE,
                                    8,
                                    EGL_RED_SIZE,
                                    8,
                                    EGL_RENDERABLE_TYPE,
                                    renderable_type,
                                    EGL_SURFACE_TYPE,
                                    EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
                                    EGL_NONE};

    if(g_display == EGL_NO_DISPLAY) {
      if((g_display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY){
        DLOG(ERROR) << "eglGetDisplay fail with error" << GetLastEGLErrorString();
        return false;
      }
      if(!eglInitialize(g_display, 0, 0)){
        DLOG(ERROR) << "eglInitialize fail with error" << GetLastEGLErrorString();
        return false;
      }
    }

    EGLint num_configs;

    if(!eglChooseConfig(g_display, config_attribs_8888, &config_, 1, &num_configs)) {
      LOG(ERROR) << "eglChooseConfig failed with error "
               << GetLastEGLErrorString();
      return false;
    }

    surface_ = eglCreateWindowSurface(g_display, config_, window_, NULL);
    return true;
  }

  void NativeViewGLSurfaceEGL::SwapBuffers() {
    eglSwapBuffers(GetDisplay(), surface_);
  }
}