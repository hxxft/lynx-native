// Copyright 2017 The Lynx Authors. All rights reserved.

#include "gl/gl_context_egl.h"
#include "gl/gl_surface.h"
#include "gl/egl_util.h"

#include "base/log/logging.h"

namespace gl {
GLContextEGL::GLContextEGL(GLShareGroup* share_group) {}

bool GLContextEGL::Initialize(GLSurface* compatible_surface) {
  EGLint context_client_version = 2;
  const EGLint kContextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION,
                                       context_client_version, EGL_NONE};
  display_ = compatible_surface->GetDisplay();
  config_ = compatible_surface->GetConfig();

  if (!eglBindAPI(EGL_OPENGL_ES_API)) {
    LOG(ERROR) << "eglBindApi failed with error " << GetLastEGLErrorString();
    return false;
  }

  context_ = eglCreateContext(
      display_, config_, share_group() ? share_group()->GetHandle() : nullptr,
      kContextAttributes);

  if (!context_) {
    LOG(ERROR) << "eglCreateContext failed with error "
               << GetLastEGLErrorString();
    return false;
  }

  return true;
}

bool GLContextEGL::MakeCurrent(GLSurface* surface) {
  if (!eglMakeCurrent(display_, surface->GetHandle(), surface->GetHandle(),
                 context_)) {
    return false;
                 }
                 return true;
}
void GLContextEGL::ReleaseCurrent(GLSurface* surface) {}
bool GLContextEGL::IsCurrent(GLSurface* surface) {}
}  // namespace gl