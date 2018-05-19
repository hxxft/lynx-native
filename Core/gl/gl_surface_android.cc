// Copyright 2017 The Lynx Authors. All rights reserved.

#include "gl/gl_surface.h"
#include "gl/gl_surface_egl.h"

namespace gl {
base::ScopedRefPtr<GLSurface> GLSurface::CreateViewGLSurface(
    AcceleratedWidget window) {
  base::ScopedRefPtr<GLSurface> surface;
  if (window != kNullAcceleratedWidget) {
    surface = new NativeViewGLSurfaceEGL(window);
    if (!surface->Initialize())
      surface = nullptr;
  }
  return surface;
}
}  // namespace gl