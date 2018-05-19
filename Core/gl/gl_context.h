// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_GL_GL_CONTEXT_H_
#define LYNX_GL_GL_CONTEXT_H_

#include "gl/gl_share_group.h"

#include "base/ref_counted_ptr.h"

namespace gl {
class GLSurface;
class GLContext : public base::RefCountPtr<GLContext> {
  public:
  virtual bool Initialize(GLSurface* compatible_surfaced) = 0;
  virtual bool MakeCurrent(GLSurface* surface) = 0;
  virtual void ReleaseCurrent(GLSurface* surface) = 0;
  virtual bool IsCurrent(GLSurface* surface) = 0;

  static base::ScopedRefPtr<GLContext> CreateGLContext(
      GLShareGroup* share_group,
      GLSurface* compatible_surface);
};
}  // namespace gl

#endif  // LYNX_GL_GL_CONTEXT_H_
