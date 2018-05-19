// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_GL_GL_EGL_CONTEXT_H_
#define LYNX_GL_GL_EGL_CONTEXT_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "gl/gl_context.h"
#include "gl/gl_share_group.h"

typedef void* EGLContext;
typedef void* EGLDisplay;
typedef void* EGLConfig;

namespace gl {
class GLSurface;
class GLContextEGL : public GLContext {
 public:
  explicit GLContextEGL(GLShareGroup* share_group);
  bool Initialize(GLSurface* compatible_surface) override;
  bool MakeCurrent(GLSurface* surface) override;
  void ReleaseCurrent(GLSurface* surface) override;
  bool IsCurrent(GLSurface* surface) override;

  GLShareGroup* share_group() {
    return share_group_;
  } 

 private:
  EGLContext context_;
  EGLDisplay display_;
  EGLConfig config_;

  GLShareGroup* share_group_;
};
}  // namespace gl

#endif