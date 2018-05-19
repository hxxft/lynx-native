// Copyright 2017 The Lynx Authors. All rights reserved.

#include "gl/gl_context_egl.h"

#include "gl/gl_surface.h"

namespace gl {
base::ScopedRefPtr<GLContext> GLContext::CreateGLContext(
    GLShareGroup* share_group,
    GLSurface* compatible_surface) {
  base::ScopedRefPtr<GLContext> context;
  if (compatible_surface->GetHandle()) {
    context = new GLContextEGL(share_group);
  }
  if (!context->Initialize(compatible_surface))
    context = nullptr;

  return context;;
}
}